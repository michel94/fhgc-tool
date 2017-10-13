#include <bits/stdc++.h>
using namespace std;

#include "nmi.hpp"
#include "jaccard.hpp"

int main(int argc, char** argv) {
	set<int> a, b;

	if(argc < 3){
		printf("Usage: ./compareClusters <clusterFile1> <clusterFile2> [--nmi/--jaccard]\n");
		return 1;
	}
	bool useNMI = true;
	string cl1 = "", cl2 = "";
	for(int i=1; i<argc; i++){
		string s = argv[i];
		string b = s.substr(2, string::npos);
		if(s.substr(0, 2) == "--"){
			if(b == "nmi"){
				useNMI = true;
			}else if(b == "jaccard"){
				useNMI = false;
			}
		}else{
			if(cl1 == "")
				cl1 = argv[i];
			else
				cl2 = argv[i];
		}
	}
	cout << "Using " << (useNMI ? "NMI" : "Jaccard Coefficient") << endl;
	cout << "Files: " << cl1 << "; " << cl2 << endl;
	if(useNMI)
		cout << "NMI: " << calcNMI(cl1.c_str(), cl2.c_str()) << endl;
	else{
		cout << "Jaccard Coefficient: " << calcJaccardCoef(cl1.c_str(), cl2.c_str()) << endl;
	}
	return 0;
}

