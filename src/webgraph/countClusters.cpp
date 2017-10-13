#include <map>
#include <set>
#include <iostream>
#include <string>
#include <string.h>
using namespace std;

typedef pair<int, int> pii;

int numComm = 20;
int main(int argc, char** argv) {
	int n;
	if(argc < 2){
		cout << "Usage: ./countClusters file.txt [number of larger communities]" << endl;
		return 1;
	}
	FILE* f = freopen(argv[1], "r", stdin);
	if(f == NULL){
		cout << "Could not open " << argv[1] << ": File not found." << endl;
		return 1;
	}
	if(argc > 2){
		numComm = atoi(argv[2]);
	}
	bool outputLargerCommunities = true;
	if(argc > 3){
		if(strcmp(argv[3], "--no-comm-sizes") == 0)
			outputLargerCommunities = false;
	}

	map<int, int> m;
	int size = 0;
	int line = 0;
	while(cin >> n){
		size++;
		line++;
		if(m.find(n) == m.end()){
			m[n] = 1;
		}else{
			m[n]++;
		}
	}
	set<pii > q;
	for(auto it = m.begin(); it != m.end(); it++){
		if(it->second > q.begin()->first || (int)q.size() < numComm ){
			if((int)q.size() == numComm)
				q.erase(q.begin());
			q.insert(pii(it->second, it->first));
		}
	}
	if(outputLargerCommunities)
		cout << numComm << " larger clusters:" << endl;
	int c = 0;
	for(auto it = q.rbegin(); it != q.rend(); it++){
		if(outputLargerCommunities)
			cout << it->first << ": " << it->second << endl;
		c += it->first;
	}

	printf("Number of communities: %d\n", (int) m.size());
	cout << c << endl;
	printf("Percentage of nodes in the larger %d communities: %lf%%\n", numComm, 100 * (c / (double) size) );

	return 0;
}
