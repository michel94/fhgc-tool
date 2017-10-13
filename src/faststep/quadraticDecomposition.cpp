#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

#include "utils/graph.h"
#include "BinaryDecomposition.h"
#include "GradientN2.h"

using namespace std;

int main(int argc, const char *argv[]) {
  /**
     PARSE ARGUMENTS
   **/
  if (argc < 4) {
    printf("Usage: %s <DecompositionType> <FILE> <#Factors> [min iters] [absolute stoppage] [relative stoppage]\n", argv[0]);
    puts("\n ATTENTION: Results are saved to the 'communities/' folder!!\n");
    puts("\t<DecompositionType>: AA if using a symmetric decomposition (row and columns will have the same values), AB otherwise.");
    puts("\t<FILE>: 0-index edge list file. The first line should represent the size of the matrix.");
    puts("\t<#Factors>: Number of factors of the decomposition.");
    puts("\t[min iters] (optional): Minimum number of gradient descent iterations. (DEFAULT = 5)");
    puts("\t[absolute stoppage] (optional): Gradient descent will continue while the absolute error improvement is above this threshold. (DEFAULT = 10)");
    puts("\t[relative stoppage] (optional): Gradient descent will continue while the relative improvement is above this threshold. (DEFAULT = 0.01)");
    exit(-1);
  }
  bool equiv = (strcmp(argv[1], "AA") == 0);
  int factors = atoi(argv[3]);
  int iters = (argc > 4?atoi(argv[4]):5);
  double absstop = (argc > 5?atof(argv[5]):10);
  double relstop = (argc > 6?atof(argv[6]):0.01);

  Graph *g = new Graph(true, true, true);
  g->readNormalizedEL(argv[2]);

  BinaryDecomposition *bd = new GradientN2(g, factors, equiv, 20, 1.0);
  printf("Running quadratic %s decomposition using %d factors, min. iters=%d, abs. stop=%lf, rel. stop=%lf\n", (equiv?"AA": "AB"), factors, iters, absstop, relstop);

  /**
     CALCULATING
  **/
  Decomposition *d = bd->calc(iters, absstop, relstop);

  /**
     SAVING TO FILE
  **/
  char labelsfile[256];
  sprintf(labelsfile, "labels-%d", factors);
  d->save("communities/", labelsfile);
 
  return 0;
}
