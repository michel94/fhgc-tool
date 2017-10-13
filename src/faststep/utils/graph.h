#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <set>
#include <vector>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <cfloat>

using std::vector;
using std::set;
using std::map;
using std::max;

double log1pex(double x);

class Graph {
public:
  Graph(bool setEL, bool vectorEL, bool AM);
  void readNormalizedEL(const char *filename);
  void calcDegrees();
  void writeSubMatrixEL(const map<int, int>[2], const char *filename);
  map<int, int> sortByDegree(const size_t mode, const set<int> elems[]);

  
  void addEdge(int a, int b);
  bool hasEdge(int a, int b);
  
  inline size_t getN() { return n; }
  size_t getM();
  size_t getNEdges();
  
  size_t getDegree(int mode, int k);
  size_t getMaxDegree(int k);

  inline size_t numNeighbors(int k) { return vEL[k].size(); }
  inline int getNeighbor(int node, int k) { return vEL[node][k]; }

  void init(int n, int m);

private:
  size_t n, m; 
  size_t nedges;

  bool hasSetEL;
  bool hasVectorEL;
  bool hasAM;


  bool *AM;
  inline void setAM(int a, int b, bool val);
  inline bool getAM(int a, int b);

  vector<vector<int> > vEL;
  vector<set<int> > sEL;
  vector<size_t> degrees[2];
  size_t maxDegree[2];

  void initializeStructs();
};

#endif
