#pragma once

#include <utility>
#include <vector>
#include <map>
#include <unordered_map>
#include <math.h>
#include <string.h>
using namespace std;

typedef pair<int, int> pii;

struct Triple{
	int xCount, yCount, totalCount;
};

void readClusters(const char* filename, vector<vector<int>*>& clusters, vector<int>& labels);
void readClusters(const char* filename, vector<vector<int>*>& clusters, vector<vector<int> >& labels);
void deleteClusters(vector<vector<int>*>& clusters);
Triple calcCounts(vector<vector<int>*>& clusters1, vector<vector<int>*>& clusters2,
				vector<vector<int> >& labels1, vector<vector<int> >& labels2,
				map<pii, int>& countXY, vector<int>& countX, vector<int>& countY);