#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

string SRC = "src/";
string WG_SRC = SRC + "webgraph/";

struct Execution{
	FILE* f;
	int retCode;
	Execution(FILE* f, int retCode){
		this->f = f;
		this->retCode = retCode;
	}
};

bool startswith(const string& path, const string& st){
	return path.substr(0, st.size()) == st;
}

void help(){
	cout << "Usage: ./fhgc <command>" << endl;
	cout << "Valid commands:" << endl
		 << "   create-dataset <dataset-in>/<raw-data> <dataset-out> [options]" << endl
		 << "   clusters faststep/louvain/LLP <dataset> <output-folder> [options]" << endl
		 << "   labels faststep/louvain/LLP <dataset> <output-file> [options]" << endl
		 << "   clusterings2labels <dataset> <clusterings-prefix> <output-file> [options]" << endl
		 << "   reorder <dataset-in> <dataset-out> <indexes>" << endl
		 << "   compare-clusters <cluster-file-1> <cluster-file-2> [options]" << endl
		 << "   compare-clusters-list <regex-clusters-1> <regex-clusters-2> [options]" << endl
		 << "   size <dataset> [<more datasets>]" << endl
		 << "   countClusters <cluster-file>" << endl
		 << "   help" << endl;
}

void readfile(FILE* f){
	const int SIZE = 1000;
	char str[SIZE+1];
	while(fgets(str, SIZE, f) != NULL){
		cout << str;
	}
}

void readfile(FILE* f, ofstream& ost){
	const int SIZE = 1000;
	char str[SIZE+1];
	while(fgets(str, SIZE, f) != NULL){
		ost << str;
		cout << str;
	}
	ost.close();
}

Execution exec(string s){
	FILE* f;
	f = popen(s.c_str(), "r");
	readfile(f);
	int ret = pclose(f);
	return Execution(f, ret);
}

Execution exec(string s, ofstream& ost){
	FILE* f;
	f = popen(s.c_str(), "r");
	readfile(f, ost);
	int ret = pclose(f);
	return Execution(f, ret);
}

void ls(string path, vector<string>& files){
	string s = string("ls -1 ") + path;
	FILE* f;
	f = popen(s.c_str(), "r");
	const int SIZE = 1000;
	char str[SIZE+1];
	while(fgets(str, SIZE, f) != NULL){
		string fname(str);
		fname = fname.substr(0, fname.size()-1);
		files.push_back(fname);
	}
}

string join(vector<string>& l, string d){
	string s;
	for(int i=0; i<(int)l.size()-1; i++)
		s += l[i] + d;
	s += l[l.size()-1];
	return s;
}

int main(int argc, char** argv){
	if(argc < 2){
		help();
		return 0;
	}
	string command = argv[1];
	if(command == "create-dataset"){
		if(argc < 4){
			cout << "Usage: ./fhgc create-dataset" << endl
			     << "   create-dataset <raw-data> <dataset-out> [options]" << endl
			     << "<raw-data>: file with one line per edge of the graph, with two integers per line." << endl
			     << "    By default, the vertex indexes start at one and the edges are considered undirected." << endl
			     << "[options]:" << endl
			     << "   --shuffle: randomly shuffle the indexes of the vertices of the graph" << endl
			     << "   --sub0: assume that indexes of nodes of the input start at zero" << endl
			     << "   --directed: the edges are considered as directed links" << endl;
			return 1;
		}else{
			string s = WG_SRC + string("create-dataset.new.sh ");
			for(int i=2; i<argc; i++){
				s += argv[i];
				s += " ";
			}
			cout << "Running: " << s << endl;
			int ret = exec(s).retCode;
			
			if(ret != 0)
				return 1;
		}
	}else if(command == "clusters" || command == "labels"){
		bool CLUSTERS = false;
		if(command == "clusters")
			CLUSTERS = true;
		if(argc < 5){
			cout << "Usage: ./fhgc " << command << endl;
			if(CLUSTERS)
				cout << "   " << command << " faststep/louvain/LLP <dataset> <output-folder> [options]" << endl;
			else
				cout << "   " << command << " faststep/louvain/LLP <dataset> <output-file> [options]" << endl;
			return 1;
		}else{
			string method = argv[2];
			string s;
			if(method == "LLP"){
				s = WG_SRC + string("LLP.sh ");
				if(CLUSTERS)
					s += argv[3] + string(" /tmp/labels.txt -l ") + argv[4] + "/communities";
				else
					s += argv[3] + string(" ") + argv[4];
			}else if(method == "faststep"){
				s = WG_SRC + string("faststep.sh ");
				if(CLUSTERS)
					s += argv[3] + string(" -c ") + argv[4];
				else
					s += argv[3] + string(" ") + argv[4];
			}else if(method == "louvain"){
				s = WG_SRC + string("louvain.sh ");
				if(CLUSTERS)
					s += argv[3] + string(" -c ") + argv[4];
				else
					s += argv[3] + string(" ") + argv[4];
			}else{
				cout << "Invalid method " << method << endl;
				return 1;
			}
			cout << "Running: " << s << endl;
			int ret = exec(s).retCode;
			if(ret != 0)
				cout << "Method failed with error: " << ret << endl;
			return ret != 0;
		}
	}else if(command == "reorder"){
		if(argc < 5){
			cout << "Usage: ./fhgc " << command << endl;
			cout << "   reorder <dataset-in> <dataset-out> <indexes>" << endl;
			return 1;
		}else{
			string s = WG_SRC + "transform.sh " + argv[2] + " " + argv[3] + " " + argv[4];
			return exec(s).retCode;
		}
	}else if(command == "compare-clusters"){
		if(argc < 4){
			cout << "Usage: ./fhgc compare-clusters" << endl
				 << "   compare-clusters <cluster-file-1> <cluster-file-2>" << endl;
			return 1;
		}else{
			string s = WG_SRC + "compareClusters " + argv[2] + " " + argv[3];
			if(argc >= 5)
				s += string(" ") + argv[4];
			cout << s << endl;
			return exec(s).retCode;
		}
	}else if(command == "compare-clusters-list"){
		if(argc < 4){
			cout << "Usage: ./fhgc compare-clusters-list" << endl
				 << "   <file-list-1> <file-list-2> [<more-file-lists>]" << endl
				 << "[options]:" << endl
			     << "   --matrix <method1> <method2>: print the results in matrix form. Accepted values for method1 and method2 are \"LLP\", \"Louvain\" or \"Faststep\" " << endl;
			return 1;
		}
		bool printMatrix = false;
		bool genImage = false;
		string method1, method2;
		vector<vector<string> > filelists;
		string args = "--nmi";
		for(int i=2; i<argc; i++){
			if(startswith(argv[i], "--")){
				if(string(argv[i]) == "--matrix"){
					if(i+2 >= argc){
						cout << "--matrix option expects the method names" << endl;
						return 1;
					}
					cout << "Printing matrix of results for methods " << argv[i+1] << " and " << argv[i+2] << endl;
					method1 = argv[i+1];
					method2 = argv[i+2];
					i += 2;
					printMatrix = true;
				}else if(string(argv[i]) == "--jaccard"){
					args = "--jaccard";
				}else if(string(argv[i]) == "--nmi"){
					args = "--nmi";
				}else if(string(argv[i]) == "--all-jaccards"){
					args = "--all-jaccards";
				}else if(string(argv[i]) == "--onmi"){
					args = "--onmi";
				}else if(string(argv[i]) == "--image"){
					genImage = true;
				}
			}else{
				string s = string("ls -1 ") + argv[i];
				FILE* f;
				f = popen(s.c_str(), "r");
				const int SIZE = 1000;
				char str[SIZE+1];
				filelists.resize(filelists.size()+1);
				while(fgets(str, SIZE, f) != NULL){
					string fname(str);
					fname = fname.substr(0, fname.size()-1);
					filelists[filelists.size()-1].push_back(fname);
				}
			}
		}
		string s = WG_SRC + "comparisonMatrix " + args + " ";
		for(int i=0; i<(int)filelists.size(); i++){
			s += "\"";
			for(int j=0; j<(int)filelists[i].size(); j++){
				s += filelists[i][j];
				if(j<(int)filelists[i].size() - 1)
					s += "|";
			}
			s += "\"";
			s += " ";
		}
		if(!printMatrix){
			return exec(s).retCode;
		}else{
			ofstream out("/tmp/comp.txt");
			out << method1 << " with " << method2 << endl;
			int r = exec(s, out).retCode;
			if(r != 0)
				return r;
			string outputImage = "";
			if(genImage)
				outputImage = " --image";
			return exec(string("python ") + WG_SRC + "genCompMatrix.py /tmp/comp.txt" + outputImage).retCode;
		}
	}else if(command == "size"){
		if(argc < 3){
			cout << "Usage: ./fhgc size" << endl
				 << "   size <dataset> [<more datasets>]" << endl;
			return 1;
		}
		bool showPercentage = false;
		string firstGraph;
		int firstSize = 1;
		vector<string> args;
		for(int i=2; i<argc; i++){
			string arg = argv[i];
			if(startswith(arg, "--")){
				if(arg == "--percent")
					showPercentage = true;
				continue;
			}
			if(firstGraph == "")
				firstGraph = arg;
			args.push_back(arg);
		}

		for(int i=0; i<(int)args.size(); i++){
			string arg = args[i];
			string s = string("ls -s --block-size=KB ") + arg + string(".graph");
			FILE* f;
			f = popen(s.c_str(), "r");
			const int SIZE = 1000;
			char str[SIZE+1];
			while(fgets(str, SIZE, f) != NULL){
				stringstream ss(str);
				string s1, s2;
				ss >> s1 >> s2;
				int fSize;
				istringstream(s1.substr(0, s1.size()-2)) >> fSize;
				if(firstGraph == arg)
					firstSize = fSize;
				if(showPercentage && firstGraph != arg)
					cout << arg << ": " << s1 << " " << 100 * (1.0 - (double)fSize / firstSize) << endl;
				else
					cout << arg << ": " << s1 << endl;
			}
		}
	}else if(command == "clusterings2labels"){
		if(argc < 5){
			cout << "Usage: ./fhgc clusterings2labels" << endl
				 << "   clusterings2labels <dataset> <clusterings-prefix> <output-file> [--hierarchical]" << endl
				 << "    - Generates a reordering for the graph using external data, either clusterings with multiple levels of granularity or hierarchical clusterings" << endl << endl
				 << "Parameters:" << endl
				 << "   - <clusterings-prefix>: the prefix of the files containing the clusterings,"
				 << "e.g., \"communities-\" which corresponds to \"communities-0.txt\", \"communities-1.txt\", \"communities-2.txt\", etc. (files must start at zero and any file suffix is accepted)" << endl
				 << "   - --hierarchical: assumes that the communities are hierarchical"
				 << endl;
			return 1;
		}
		vector<string> files;
		string prefix = argv[3];
		ls(string(" ") + prefix + "*", files);
		vector<string> ordered(files.size());
		int useOrder = true;
		for(int i=0; i<(int)files.size(); i++){
			cout << files[i] << endl;
			string e = files[i].substr(prefix.size(), string::npos);
			stringstream ss(e);
			int n; ss >> n;
			if(n < (int) ordered.size())
				ordered[n] = files[i];
			else
				useOrder = false;
		}
		for(int i=0; i<(int)ordered.size(); i++)
			if(ordered[i].size() == 0){
				useOrder = false;
				break;
			}
		string clusters = "";
		if(useOrder){
			clusters = join(ordered, ",");
			cout << "Using ordered clusterings" << endl;
		}else{
			clusters = join(files, ",");
			cout << "Can't use ordered clusterings" << endl;
		}

		string s = string("java -cp src/law+faststep/jars/runtime/'*':src/law+faststep/build it.unimi.dsi.law.graph.LayeredLabelPropagation ") + argv[2] + " " + argv[4] + " -l my-labels --inClusters " + clusters;
		exec(s);
		s = string("src/webgraph/labels2txt.sh ") + argv[4] + string("; mv ") + argv[4] + string(".txt ") + argv[4];
		exec(s);

	}else if(command == "countClusters"){
		if(argc < 3){
			cout << "Usage: ./fhgc countClusters" << endl
				 << "   countClusters <clusters-file>" << endl;
			return 1;
		}
		return exec(WG_SRC + "countClusters " + argv[2]).retCode;
	}else if(command == "help"){
		help();
	}else{
		cout << "Invalid command" << endl;
		help();
	}

	return 0;

}
