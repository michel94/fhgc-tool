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

const int FACTORS = 1;
const int NDIVISIONS = 2;
double TAU = 20.0;

void writeLabels(Subgraph* sg){
  int comm = getCommId();
  for(int i=0; i<(int)sg->g->getN(); i++){
    //assert(NodeCommunities[nodeIndex[i]] == -1);
    NodeCommunities[sg->nodeIndex[i]] = comm;
    NodeLabels[sg->nodeIndex[i]] = getLabel();
  }
}

void writeTree(Tree* root){
  if(root->children.size() == 0)
    writeLabels(root->graph);
  else
    for(int i=0; i<(int)root->children.size(); i++)
      writeTree(root->children[i]);
}

int firstIteration = true;

vector<Subgraph*> divide(vector<int>& nodeIndex, Graph* g){
  printf("Graph with size %d\n", (int)g->getN());
  int samples = (int)g->getNEdges() * FACTORS;
  BinaryDecomposition *bd = new ApproxGradient(g, FACTORS, equiv, TAU, samples, pratio);
  printf("Running approximate %s decomposition using %d factors, min. iters=%d, abs. stop=%lf, rel. stop=%lf, samples=%d\n", 
    (equiv?"AA": "AB"), FACTORS, iters, absstop, relstop, samples);

  Decomposition *d = bd->calc(iters, absstop, relstop);
  int maxIndex = 0;
  double mbk = maxBk(d, 0, maxIndex);
  double threshold = 2 * TAU / (FACTORS * mbk);
  delete bd;

  vector<int> communitySize;
  map<int, int> nodeComm;
  vector<int> newNodeId;
  Graph** subgraphs = new Graph*[NDIVISIONS];

  vector<vector<int> > nodeIndexes;
  nodeIndexes.resize(NDIVISIONS);
  for(int i=0; i<NDIVISIONS; i++) nodeIndexes[i].resize(g->getN());

  communitySize.resize(NDIVISIONS);
  newNodeId.resize(g->getN());
  long double sumV = 0;
  int n = g->getN();
  pair<double, int>* arr = new pair<double, int>[n];
  for(int i=0; i<n; i++){
    arr[i] = make_pair(factorAt(d, i, 0), i);
    sumV += arr[i].first;
  }
  sort(arr, arr+n, std::greater<pair<double, int> >());
  
  //sumV /= g->getN();
  double curSum = 0;
  double cutOff = sumV * 0.5;
  for(int j=0; j<n; j++){
    double v = arr[j].first;
    int i = arr[j].second;
    curSum += v;
    int hf = 1;
    if(curSum > cutOff)
      hf = 0;
    
    //double S = d->score(i, maxIndex);
    newNodeId[i] = communitySize[hf];
    communitySize[hf]++;
    nodeComm[i] = hf;
  }
  if(firstIteration){
    FILE* matr = fopen("matrix.txt", "w");
    for(int i=0; i<(int)g->getN(); i++){
      fprintf(matr, "%lf\n", factorAt(d, i, 0));
    }
    fprintf(matr, "%lf\n", cutOff);
    firstIteration = false;
  }

  printf("threshold: %lf, maxBk: %lf, maxIndex: %d\n", threshold, mbk, maxIndex);
  for(int i=0; i < NDIVISIONS; i++){
    subgraphs[i] = new Graph(true, true, false);
    int s = communitySize[i];
    subgraphs[i]->init(s, s);
    printf("Subgraph %d with size %d\n", i, s);
  }
  for(int node=0; node < (int)g->getN(); node++){
    nodeIndexes[nodeComm[node]][newNodeId[node]] = nodeIndex[node];
    for(int j=0; j < (int)g->numNeighbors(node); j++){
      int neigh = g->getNeighbor(node, j);
      if(nodeComm[node] == nodeComm[neigh])
        subgraphs[ nodeComm[node] ]->addEdge(newNodeId[node], newNodeId[neigh]);
    }
  }
  
  /*char filename[256];
  sprintf(filename, "labels-%d", FACTORS);
  d->save("communities/", filename);
  */
  vector<Subgraph*> subproblems;
  delete d;
  for(int i=0; i<NDIVISIONS; i++){
    subproblems.push_back(new Subgraph(nodeIndexes[i], subgraphs[i]));
  }
  return subproblems;
}

void saveToFile(Graph* g, int commSize){
  char labelsfile[128];
  char communitiesfile[128];
  sprintf(labelsfile, "communities/labels-%ddiv.txt", commSize);
  sprintf(communitiesfile, "communities/communities-%ddiv.txt", commSize);
  FILE *foutLabels = fopen(labelsfile, "w");
  FILE *foutComms = fopen(communitiesfile, "w");
  for (size_t j = 0; j < g->getM(); j++){
    fprintf(foutLabels, "%d\n", NodeLabels[j]);
    fprintf(foutComms, "%d\n", NodeCommunities[j]);
  }
  fclose(foutLabels);
  fclose(foutComms);
  printf("Wrote communities-%ddiv\n", commSize);
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
  iters = (argc > 4 ? atoi(argv[4]) : 100);
  absstop = (argc > 5 ? atof(argv[5]) : 0.001);
  relstop = (argc > 6 ? atof(argv[6]) : 0.0001);
  
  samples = (argc > 7 ? atoi(argv[7]) : 20*g->getNEdges() );

  NodeCommunities = new int[size];
  NodeLabels = new int[size];
  fill(NodeCommunities, NodeCommunities + size, -1);
  fill(NodeLabels, NodeLabels + size, -1);
  
  vector<int> nodeIndex(g->getN());
  for(int i=0; i<(int)g->getN(); i++)
    nodeIndex[i] = i;

  /**
    SAVING TO FILES
  **/
  Tree* root = new Tree();

  priority_queue<Subgraph*, std::vector<Subgraph*>, Compare> q;
  sort(maxCommSizes.begin(), maxCommSizes.end());
  Subgraph* rootSG = new Subgraph(nodeIndex, g);
  rootSG->tree = root;
  root->graph = rootSG;
  q.push(rootSG);
  while(maxCommSizes.size() > 0){
    MAX_COMM_SIZE = maxCommSizes[maxCommSizes.size()-1];
    maxCommSizes.pop_back();
    if(q.size() > 0 && q.top()->s < MAX_COMM_SIZE){
      continue;
    }

    cout << "Communities with size " << MAX_COMM_SIZE << endl;

    COMMID = 0;
    LABEL = 0;

    while(q.size() > 0 && q.top()->s > MAX_COMM_SIZE){
      Subgraph* sg = q.top();
      q.pop();
      vector<Subgraph*> subgraphs = divide(sg->nodeIndex, sg->g);
      int zeros = 0;
      for(int i=0; i<(int)subgraphs.size(); i++){
        if(subgraphs[i]->size() == 0)
          zeros++;
      }
      printf("Empty subgraphs: %d, subgraphs: %d\n", zeros, (int)subgraphs.size());
      if(zeros + 1 == (int)subgraphs.size()){ // only one non empty graph (the same graph), nothing more to do
        printf("No push to queue, q size: %d\n", (int)q.size());;
      }else{
        for(int i=0; i<(int)subgraphs.size(); i++){
          q.push(subgraphs[i]);
          sg->tree->add(subgraphs[i]->tree);
        }
        
      }
    }

    writeTree(root);
    saveToFile(g, MAX_COMM_SIZE);

  }

}
