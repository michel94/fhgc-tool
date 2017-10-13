#ifndef __BINARYN2_H__
#define __BINARYN2_H__

#include <algorithm>
#include <cmath>
#include <cfloat>

#include "utils/graph.h"
#include "BinaryDecomposition.h"

class GradientN2 : public BinaryDecomposition {
public:
  GradientN2(Graph *_g, int factors, bool _equiv, double _r, double lambda);
  ~GradientN2();
  
  double getR();
  Decomposition* calc(size_t iter, double absstop, double relstop);
  void calcGradient(Decomposition *oth, int f1, double gradA[], double gradB[]);
  void calcSum(Decomposition *d);

private:
  Graph *g;  
  Decomposition *cur, *oth;

  double sum[8000][8000];

  bool equiv;
  double r;
  double l;
  int f;
};

#endif
