#include <iostream>
using namespace std;

#include "nmi.hpp"

inline long double absD(long double x){
	return x > 0.0 ? x : -x;
}

namespace{
	vector<vector<int>*> clusters1, clusters2;
	vector<vector<int> > labels1, labels2;
	map<pii, int> countXY;
	vector<int> countX, countY;	
}

long double LOG2(long double x){
	return absD(x) < 0.000000001 ? 0 : log(x) / log(2);
}


long double entropy(vector<int>& count, long double n){
	long double s = 0;
	for(int i=0; i < (int)count.size(); i++){
		s += (count[i] / n) * LOG2(count[i] / n);
	}
	return - s;
}

long double calcNMI(const char* file1, const char* file2){
	readClusters(file1, clusters1, labels1);
	readClusters(file2, clusters2, labels2);

	Triple t = calcCounts(clusters1, clusters2, labels1, labels2, countXY, countX, countY);

	long double entX = entropy(countX, t.xCount);
	long double entY = entropy(countY, t.yCount);
	long double I = 0;

	for(auto it = countXY.begin(); it != countXY.end(); it++){
		int x = it->first.first;
		int y = it->first.second;
		int xy = it->second;
		I += xy * (LOG2(xy) - LOG2(t.totalCount) - LOG2(countX[x]) - LOG2(countY[y]) + LOG2(t.xCount) + LOG2(t.yCount) );
	}
	I /= t.totalCount;
	
	//cerr << file1 << " " << file2 << " " << entX << " " << entY << " " << I << endl;
	//fprintf(stderr, "%s %s: H(X)=%lf H(Y)=%lf I(X;Y)=%lf\n", file1, file2, entX, entY, I);
	long double nmi = 1.0;
	if(entX + entY > 0.000001)
		nmi = (2 * I) / (entX + entY);

	deleteClusters(clusters1);
	deleteClusters(clusters2);

	return nmi;
}

