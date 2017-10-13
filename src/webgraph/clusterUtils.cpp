#include <sstream>
#include <fstream>
#include <iostream>
#include <cassert>
#include "clusterUtils.hpp"

void readClusters(const char* filename, vector<vector<int>*>& clusters, vector<int>& labels){
	unordered_map<int, vector<int>* > m;
	int i = 0;
	ifstream f(filename);
	int n;
	while(f >> n){
		if(m.find(n) == m.end())
			m[n] = new vector<int>;
		m[n]->push_back(i++);
	}
	int size = i;
	labels.resize(size);
	i = 0;
	for(auto it=m.begin(); it != m.end(); it++, i++){
		clusters.push_back(it->second);
		vector<int> *cluster = it->second;
		for(auto itc = cluster->begin(); itc != cluster->end(); itc++)
			labels[*itc] = i;
	}
	f.close();
}

void readClusters(const char* filename, vector<vector<int>*>& clusters, vector<vector<int> >& labels){
	labels.clear();
	clusters.clear();

	map<int, vector<int>* > m;
	int i = 0;
	ifstream f(filename);
	int n;
	string line;
	while(getline(f, line)){
		istringstream lineStream(line);
		while(lineStream >> n){
			if(m.find(n) == m.end())
				m[n] = new vector<int>;
			m[n]->push_back(i);
		}
		i++;
	}
	int size = i;
	labels.resize(size);
	i = 0;
	for(auto it=m.begin(); it != m.end(); it++, i++){
		clusters.push_back(it->second);
		vector<int> *cluster = it->second;
		for(auto itc = cluster->begin(); itc != cluster->end(); itc++)
			labels[*itc].push_back(i);
	}
	f.close();
}

void deleteClusters(vector<vector<int>*>& clusters){
	for(auto it=clusters.begin(); it != clusters.end(); it++)
		delete *it;
}

Triple calcCounts(vector<vector<int>*>& clusters1, vector<vector<int>*>& clusters2,
				vector<vector<int> >& labels1, vector<vector<int> >& labels2,
				map<pii, int>& countXY, vector<int>& countX, vector<int>& countY){
	countX.clear();
	countY.clear();
	countXY.clear();

	unsigned int m = max(labels1.size(), labels2.size());
	if(labels1.size() < m)
		labels1.resize(m);
	else if(labels2.size() < m)
		labels2.resize(m);
	
	countX.resize(clusters1.size());
	countY.resize(clusters2.size());
	fill(countX.begin(), countX.end(), 0);
	fill(countY.begin(), countY.end(), 0);

	int totalCount = 0, xCount = 0, yCount = 0;
	for(int i=0; i<(int)labels1.size(); i++){ // iterate over nodes
		for(int l1: labels1[i]){
			xCount++;
			for(int l2: labels2[i]){
				if(countXY.find(pii(l1,l2)) == countXY.end() )
					countXY[pii(l1,l2)] = 1;
				else
					countXY[pii(l1,l2)]++;
				totalCount++;
			}
			countX[l1]++;
		}
		for(int l2: labels2[i]){
			countY[l2]++;
			yCount++;
		}
	}
	return {xCount, yCount, totalCount};
}
