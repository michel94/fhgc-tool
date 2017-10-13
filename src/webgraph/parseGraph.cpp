#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>
using namespace std;

vector<vector<int> > inputGraph;
vector<vector<int> > tmpGraph;
int n;

int main(int argc, const char *argv[]) {
	int a, b;
	bool shuffle = false, directed = false;
	int sub = 1;
	FILE* fsFile=NULL, *wgFile=NULL;
	if(argc < 2){
		printf("Usage: ./parseGraph [webgraph=<webgraph-ascii-file>] [faststep=<faststep-ascii-file>]\n");
	}else{
		for(int i=0; i<argc; i++){
			string s(argv[i]);
			if(s.find("webgraph=") == 0){
				printf("Writing webgraph ascii data to %s\n", s.substr(9).c_str());
				wgFile = fopen(s.substr(9).c_str(), "w");
			}else if(s.find("faststep=") == 0){
				printf("Writing faststep ascii data to %s\n", s.substr(9).c_str());
				fsFile = fopen(s.substr(9).c_str(), "w");
			}else if(s.find("--shuffle") == 0){
				shuffle = true;
			}else if(s.find("--directed") == 0){
				printf("Reading directed graph\n");
				directed = true;
			}else if(s.find("--sub0") == 0){
				printf("Expecting zero based indexes\n");
				sub = 0;
			}
		}
	}
	string s;
	while(true){
		getline(cin, s);
		if(s[0] == '#')
			continue;
		if(cin.eof())
			break;
		istringstream is(s);
		is >> a >> b;
		if(cin.fail())
			break;
		a -= sub;
		b -= sub;
		if(a < 0 || b < 0){
			printf("Invalid node index: node indexes start at %d.\n", 1-sub);
			return 1;

		}
		int l = max(a, b);
		if( (int) inputGraph.size() <= l)
			inputGraph.resize(l + 1);
		inputGraph[a].push_back(b);
		if(!directed)
			inputGraph[b].push_back(a);
	}
	cout << "Read graph" << endl;
	if(shuffle){
		cout << "Shuffling inputGraph" << endl;
		vector<int> indexes(inputGraph.size());
		for(int i=0; i<(int)indexes.size(); i++)
			indexes[i] = i;
		random_shuffle(indexes.begin(), indexes.end());
		FILE* fInd = fopen("indexes.txt", "w");
		for(int i=0; i<(int)indexes.size(); i++)
			fprintf(fInd, "%d\n", indexes[i]);
		fclose(fInd);
		tmpGraph.resize((int)inputGraph.size());
		for(int i=0; i<(int)inputGraph.size(); i++){
			int nodeId = indexes[i];
			for(int j=0; j<(int)inputGraph[i].size(); j++){
				int neighId = indexes[inputGraph[i][j]];
				tmpGraph[nodeId].push_back(neighId);
			}
		}
	}
	vector<vector<int> >& graph = shuffle ? tmpGraph : inputGraph;
	if(wgFile != NULL){
		fprintf(wgFile, "%d\n", (int) graph.size());
		for(int i=0; i<(int)graph.size(); i++){
			sort(graph[i].begin(), graph[i].end());
			for(int j=0; j<(int)graph[i].size(); j++)
				fprintf(wgFile, "%d ", graph[i][j]);
			fprintf(wgFile, "\n");
		}
		fclose(wgFile);
	}
	if(fsFile != NULL){
		fprintf(fsFile, "%d %d\n", (int) graph.size(), (int) graph.size());
		for(int i=0; i<(int)graph.size(); i++){
			for(int j=0; j<(int)graph[i].size(); j++)
				fprintf(fsFile, "%d %d\n", i, graph[i][j]);
		}
		fclose(fsFile);
	}

	printf("Created graph files\n");
}
