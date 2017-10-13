#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <experimental/filesystem>
#include "nmi.hpp"
#include "jaccard.hpp"
#include "../overlapping-nmi/onmi.hpp"

using namespace std;
using namespace std::experimental;

enum Metric{
	NMI, ONMI, JACCARD, ALL_JACCARDS
};

string getFilename(const string& path){
	return path.substr(path.find_last_of("/")+1, path.size());
}

string metricName(Metric m){
	switch(m){
		case NMI:
			return "NMI";
		case ONMI:
			return "ONMI";
		case JACCARD:
			return "Jaccard";
		case ALL_JACCARDS:
			return "ALL_JACCARDS";
	}
	return "UNKNOWN";
}

vector<string> split(char* s, char sep){
	vector<string> v;
	int size = strlen(s);
	int j=0, i=0;
	for(; i<size; i++){
		if(s[i] == sep){
			s[i] = '\0';
			v.push_back(string(s+j) );
			j = i+1;
		}
	}
	if(i != j)
		v.push_back(s+j);

	return v;
}

int main(int argc, char** argv){
	if(argc < 3){
		printf("Usage: ./comparisonMatrix [--nmi/jaccard] < 2 or more files separated by \'|\' >\n");
		return 1;
	}
	
	Metric metric = NMI;
	long double (*metricFunc)(const char*, const char*);
	if(argc >= 3){
		string s = argv[1];
		if(s == "--jaccard"){
			metric = JACCARD;
			metricFunc = &calcJaccardCoef;
		}else if(s == "--all-jaccards"){
			metric = ALL_JACCARDS;
		}else if(s == "--nmi"){
			metric = NMI;
			metricFunc = &calcNMI;
		}else if(s == "--onmi"){
			metric = ONMI;
			metricFunc = &calcONMI;
		}
	}

	cerr << "Calculating " << metricName(metric) << endl;

	int start = 1;
	if(argv[1][0] == '-' && argv[1][1] == '-')
		start++;

	vector<vector<string> > filelists;
	for(int i=start; i<argc; i++){
		filelists.push_back(split(argv[i], '|'));
	}

	for(int i=0; i<(int)filelists.size(); i++){
		vector<string> &v1 = filelists[i];
		for(int j=i+1; j<(int)filelists.size(); j++){
			vector<string> &v2 = filelists[j];
			for(auto f1=v1.begin(); f1 != v1.end(); f1++){
				for(auto f2=v2.begin(); f2 != v2.end(); f2++){
					if(metric == ALL_JACCARDS)
						calcAllJaccards(f1->c_str(), f2->c_str());
					else
						cout << getFilename(*f1) << " " << getFilename(*f2) << ": " << metricFunc(f1->c_str(), f2->c_str()) << endl;
		    	}
		    }
		}
	}
}
