#ifndef __BINARY_DECOMPOSITION_H__
#define __BINARY_DECOMPOSITION_H__

#include <vector>
#include <cstdlib>
#include <sys/time.h>

#include "utils/graph.h"

using std::vector;

class Decomposition {
public:
  vector<vector<double> > A;
  vector<vector<double> > &B;
  vector<int> count;
  int f;
  Graph *g;
  double r;
  bool equiv;
  struct timeval t1, t2;

  Decomposition(Graph *_g, int _f, double _r, bool _equiv);
  void initRandomize(double min, double max);
  void initEpsilon(double eps);
  void initToDegrees();
  void load(const char folder[256]);
  void save(const char folder[256], const char labelsPrefix[256]);

  double score(int i, int j);
  bool predicted(int i, int j, double r);

};

class BinaryDecomposition {
public:
  virtual double getR() = 0;
  virtual Decomposition* calc(size_t iter, double absstop, double relstop) = 0;
  virtual ~BinaryDecomposition() {};

  double calcF(Decomposition*);
  virtual void calcGradient(Decomposition *oth, int f1, double gradA[], double gradB[]) = 0;
  
};

int highestFactor(Decomposition *d, int j);
double factorAt(Decomposition *d, int j, int i);
double maxBk(Decomposition* d, int k, int& index);

#endif
