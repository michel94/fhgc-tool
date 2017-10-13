#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <string>
#include <sstream>

#include "utils/graph.h"
#include "BinaryDecomposition.h"
#include "ApproxGradient.h"

using namespace std;

inline int min(int a, int b){
  return (a < b ? a : b);
}

vector<int> parseList(string str){
  vector<int> elements;
  stringstream ss(str);
  
  int i;
  while (ss >> i){
      elements.push_back(i);

      if (ss.peek() == ',')
          ss.ignore();
  }

  return elements;
}

int main(int argc, const char *argv[]) {
  /**
     PARSE ARGUMENTS
   **/
  if (argc < 4) {
    printf("Usage: %s <DecompositionType> <FILE> <#Factors> [min iters] [absolute stoppage] [relative stoppage] [samples]\n", argv[0]);
    puts("\n ATTENTION: Results are saved to the 'communities/' folder!!\n");
    puts("\t<DecompositionType>: AA if using a symmetric decomposition (row and columns will have the same values), AB otherwise.");
    puts("\t<FILE>: 0-index edge list file. The first line should represent the size of the matrix.");
    puts("\t<#Factors>: Number of factors of the decomposition.");
    puts("\t[min iters] (optional): Minimum number of gradient descent iterations. (DEFAULT = 5)");
    puts("\t[absolute stoppage] (optional): Gradient descent will continue while the absolute error improvement is above this threshold. (DEFAULT = 10)");
    puts("\t[relative stoppage] (optional): Gradient descent will continue while the relative improvement is above this threshold. (DEFAULT = 0.01)");
    puts("\t[samples] (optional): Number of samples used in approximating F. (default = #factors*number of edges)");
    exit(-1);
  }

  Graph *g = new Graph(true, true, false);
  g->readNormalizedEL(argv[2]);

  vector<int> factor_list = parseList(argv[3]);

  for(int pf=0; pf < (int)factor_list.size(); pf++){

    bool equiv = (strcmp(argv[1], "AA") == 0);
    int factors = factor_list[pf];
    printf("Number of factors: %d\n", factors);
    int iters = (argc > 4 ? atoi(argv[4]) : 20);
    double absstop = (argc > 5 ? atof(argv[5]) : 1);
    double relstop = (argc > 6 ? atof(argv[6]) : 0.001);
    
    int samples = (argc > 7 ? atoi(argv[7]) : min(factors*g->getNEdges(), 1000000) );

    double pratio = 2;
    
    BinaryDecomposition *bd = new ApproxGradient(g, factors, equiv, 2, samples, pratio);
    printf("Running approximate %s decomposition using %d factors, min. iters=%d, abs. stop=%lf, rel. stop=%lf, samples=%d\n", (equiv?"AA": "AB"), factors, iters, absstop, relstop, samples);

    /**
      CALCULATING
    **/
    Decomposition *d = bd->calc(iters, absstop, relstop);
    delete bd;
    
    /**
      SAVING TO FILES
    **/
    char filename[256];
    sprintf(filename, "labels-%d", factors);
    d->save("communities/", filename);
    FILE* fileC = fopen("clustering", "w");
    double m = -1.0;
    for(int i=0; i<(int)g->getN(); i++){
      double bv = -1000;
      int bf = 0;
      for(int f=0; f<factors; i++){
        if(d->A[f][i] > bv){
          bf = f;
          bv = d->A[f][i];
        }
      }
      fprintf(fileC, "%d\n", bf);
    }
    fclose(fileC);
    printf("Max %lf\n", m);
    
    delete d;
  }
 
  return 0;
}
