#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

vector<vector<int> > graph;

int main(int argc, char** argv){
	if(argc < 3){
		cout << "Usage: ./parseGraphTxt <input-file> <output-file>" << endl;
		return 1;
	}
	//cout << "./parseGraphTxt " << argv[1] << " " << argv[2] << endl;
	ifstream file(argv[1]);
	ofstream out(argv[2]);
	string str;
	int i=-1;
	while (getline(file, str)){
		stringstream ss(str);
		int tok;
		while(ss >> tok){
			if(i == -1)
				out << tok << " " << tok << endl;
			else
				out << i << " " << tok << endl;
		}
		i++;
	}
}
