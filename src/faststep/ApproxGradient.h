#ifndef __APPROXGRADIENT_H__
#define __APPROXGRADIENT_H__

#include <algorithm>
#include <set>
#include <map>
#include <utility>
#include <cmath>
#include <cfloat>
#include <queue>
#include "utils/graph.h"
#include "BinaryDecomposition.h"

using std::set;
using std::map;
using std::priority_queue;
using std::pair;
using std::make_pair;
using std::nth_element;
using std::min;

class ApproxGradient : public BinaryDecomposition {
public:
  ApproxGradient(Graph *_g, size_t factors, bool _equiv, double _r, int _samples, double _pratio);

  double getR();
  Decomposition* calc(size_t iter, double absstop, double relstop);

  double calcFsampling(Decomposition* d, int nsamples);
  double calcFsimulate(Decomposition* d, int nsamples);  
  Decomposition *cur, *oth;

private:
  Graph *g;  

  map<pair<size_t, size_t>, int> pEdges;  // how many communities predicted this edge and value of sij
  set<pair<size_t, size_t> > *cEdges; // what edges are predicted by this community

  bool equiv;
  double r;
  size_t f;
  int samples;
  double pratio;

  double calcFpredicted(Decomposition *d);
  void predictEdges(Decomposition *d, int factor);
  void calcGradient(Decomposition *oth, int f1, double gradA[], double gradB[]);

  void delRef(pair<size_t, size_t> k);
  void addRef(pair<size_t, size_t> k);
};

#endif
