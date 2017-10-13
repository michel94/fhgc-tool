#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cassert>
#include <string>
#include <sstream>
#include <map>
#include <iostream>

#include "utils/graph.h"
#include "BinaryDecomposition.h"
#include "ApproxGradient.h"

using namespace std;

int* NodeCommunities, *NodeLabels;
int MAX_COMM_SIZE;
int samples, size;
bool equiv;
int iters;
double absstop;
double relstop;
double pratio = 2;


vector<int> maxCommSizes = {500000, 200000, 100000, 50000, 20000, 10000, 5000, 2000, 1000, 500, 200, 100, 50, 20, 10};

class Subgraph;

class Tree{
public:
  vector<Tree*> children;
  Subgraph* graph;

  void add(Tree* tree){
    children.push_back(tree);
  }
};

class Subgraph{
public:
  vector<int> nodeIndex;
  Graph* g;
  int s;
  Tree* tree = new Tree();

  Subgraph(){
    g = NULL;
    s = 0;
    tree->graph = this;
  }
  Subgraph(vector<int> index, Graph* g){
    nodeIndex = index;
    this->g = g;
    s = g->getN();
    tree->graph = this;
  }
  const int size();
};

const int Subgraph::size(){
  return s;
}

struct Compare{
  const bool operator() (Subgraph* a, Subgraph* b){
    return a->size() < b->size();
  }
};

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

int COMMID = 0;
int LABEL = 0;
int getCommId(){
  return COMMID++;
}
int getLabel(){
  return LABEL++;
}

const int FACTORS = 4;
double TAU = 20.0;

void writeLabels(Subgraph* sg){
  int comm = getCommId();
  for(int i=0; i<(int)sg->g->getN(); i++){
    //assert(NodeCommunities[nodeIndex[i]] == -1);
    NodeCommunities[sg->nodeIndex[i]] = comm;
    NodeLabels[sg->nodeIndex[i]] = getLabel();
  }
}

void findCommunities(vector<vector<int> >& communities, Graph* g){
  printf("Graph with size %d\n", (int)g->getN());
  int samples = (int)g->getNEdges() * FACTORS;
  BinaryDecomposition *bd = new ApproxGradient(g, FACTORS, equiv, TAU, samples, pratio);
  printf("Running approximate %s decomposition using %d factors, min. iters=%d, abs. stop=%lf, rel. stop=%lf, samples=%d\n", 
    (equiv?"AA": "AB"), FACTORS, iters, absstop, relstop, samples);

  Decomposition *d = bd->calc(iters, absstop, relstop);

  cout << "Finished decomposition" << endl;
  for(int c=0; c<FACTORS; c++){
    int maxIndex = 0;
    double mbk = maxBk(d, c, maxIndex);
    double threshold = TAU / (FACTORS * mbk);
    cout << "threshold: " << threshold << " max(b_k): " << mbk << endl;
    
    for(int i=0; i<(int)g->getN(); i++){
      //int hf = highestFactor(d, i);
      double v = factorAt(d, i, c);
      double S = d->score(i, maxIndex);
      if(v >= threshold && S >= TAU)
        communities[c].push_back(i);
    }
  }
  delete bd;

}


int main(int argc, const char *argv[]) {
  /**
     PARSE ARGUMENTS
   **/
  if (argc < 3) {
    printf("Usage: %s <DecompositionType> <FILE> <#Factors> [min iters] [absolute stoppage] [relative stoppage] [samples]\n", argv[0]);
    puts("\n ATTENTION: Results are saved to the 'communities/' folder!!\n");
    puts("\t<DecompositionType>: AA if using a symmetric decomposition (row and columns will have the same values), AB otherwise.");
    puts("\t<FILE>: 0-index edge list file. The first line should represent the size of the matrix.");
    puts("\t<#Max community size>: Number of factors of the decomposition.");
    puts("\t[min iters] (optional): Minimum number of gradient descent iterations. (DEFAULT = 5)");
    puts("\t[absolute stoppage] (optional): Gradient descent will continue while the absolute error improvement is above this threshold. (DEFAULT = 10)");
    puts("\t[relative stoppage] (optional): Gradient descent will continue while the relative improvement is above this threshold. (DEFAULT = 0.01)");
    puts("\t[samples] (optional): Number of samples used in approximating F. (default = #factors*number of edges)");
    exit(-1);
  }

  Graph *g = new Graph(true, true, false);
  g->readNormalizedEL(argv[2]);
  size = g->getN();

  equiv = (strcmp(argv[1], "AA") == 0);
  iters = (argc > 4 ? atoi(argv[4]) : 12);
  absstop = (argc > 5 ? atof(argv[5]) : 0.001);
  relstop = (argc > 6 ? atof(argv[6]) : 0.0001);
  
  samples = (argc > 7 ? atoi(argv[7]) : 4*g->getNEdges() );

  vector<vector<int> > communities(FACTORS);
  findCommunities(communities, g);
  for(int c=0; c<FACTORS; c++){
    /*for(int i=0; i<(int)communities[c].size(); i++){
      cout << communities[c][i] << " ";
    }*/
    cout << "Community " << c << ": " << communities[c].size() << " elements";
    cout << endl;
  }

}
