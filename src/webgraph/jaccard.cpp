#include "jaccard.hpp"
#include <iostream>
#include <cassert>
using namespace std;

namespace{
	vector<vector<int>*> clusters1, clusters2;
	vector<vector<int> > labels1, labels2; // cluster of node i in clustering1 / clustering2
	map<pii, int> countXY; // number of common nodes between each cluster of clustering1 and each cluster of clustering2
	vector<int> countX, countY;	// number of nodes per clusters
}

void calcAllJaccards(const char* file1, const char* file2){
	readClusters(file1, clusters1, labels1);
	readClusters(file2, clusters2, labels2);

	Triple t = calcCounts(clusters1, clusters2, labels1, labels2, countXY, countX, countY);
	int n = labels1.size();
	
	vector<double> bestPerCluster(n);
	fill(bestPerCluster.begin(), bestPerCluster.end(), 0.0);
	for(auto it=countXY.begin(); it!=countXY.end(); it++){
		int cl1 = it->first.first,
			cl2 = it->first.second;
		int c = it->second;
		bestPerCluster[cl1] = max(bestPerCluster[cl1], c / ((double)countX[cl1] + countY[cl2] - c));
	}

	double s = 0;
	double ws = 0;
	int total = 0;
	for(int i=0; i<(int)countX.size(); i++){
		ws += countX[i] * bestPerCluster[i];
		s += bestPerCluster[i];
		total += countX[i];
		cout << countX[i] << " " << bestPerCluster[i] << endl;
	}
	
	cout << "Jaccard: " << s / countX.size() << endl;
	cout << "Weighted Jaccard: " << ws / total << endl;

	deleteClusters(clusters1);
	deleteClusters(clusters2);

}

long double calcJaccardCoef(const char* file1, const char* file2){
	readClusters(file1, clusters1, labels1);
	readClusters(file2, clusters2, labels2);

	Triple t = calcCounts(clusters1, clusters2, labels1, labels2, countXY, countX, countY);
	
	int n = labels1.size();

	vector<double> bestPerCluster(n);
	fill(bestPerCluster.begin(), bestPerCluster.end(), 0.0);
	for(auto it=countXY.begin(); it!=countXY.end(); it++){
		int cl1 = it->first.first,
			cl2 = it->first.second;
		int c = it->second;
		bestPerCluster[cl1] = max(bestPerCluster[cl1], c / ((double)countX[cl1] + countY[cl2] - c));
	}

	double s = 0;
	for(int i=0; i<(int)countX.size(); i++){
		s += bestPerCluster[i];
	}
	
	double jac = s / countX.size();

	deleteClusters(clusters1);
	deleteClusters(clusters2);

	return jac;
}

