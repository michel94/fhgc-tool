#include "graph.h"

#include <cstdio>

Graph::Graph(bool setEL, bool vectorEL = false, bool AM = false): hasSetEL(setEL), hasVectorEL(vectorEL), hasAM(AM) {
  this->nedges = 0;
}

size_t Graph::getM() {
  return m;
}

size_t Graph::getNEdges() {
  return nedges;
}

bool Graph::hasEdge(int a, int b) {
  if (hasAM)
    return getAM(a, b);
  if (hasSetEL)
    return sEL[a].find(b) != sEL[a].end();
  if (hasVectorEL)
    return find(vEL[a].begin(), vEL[a].end(), b) != vEL[a].end();
  puts("No graph structure found");
  exit(-1);
}

void Graph::init(int n, int m){
  this->n = n;
  this->m = m;
  initializeStructs();
}

void Graph::initializeStructs() {
  if (hasSetEL)
    sEL = vector<set<int> >(n, set<int>());
  if (hasVectorEL)
    vEL = vector<vector<int> >(n, vector<int>());
  if (hasAM) {
    AM = new bool[n*m];
    memset(AM, 0, n*m);
  }
}

void Graph::addEdge(int a, int b) {
  if (hasSetEL) 
    sEL[a].insert(b);
  if (hasVectorEL)
    vEL[a].push_back(b);
  if (hasAM)
    setAM(a, b, true);
  nedges++;
}

void Graph::readNormalizedEL(const char *filename) {
  FILE *fin = fopen(filename, "r");
  if (fin == NULL) {
    printf("Couldn't open %s\n", filename);
    return;
  }
    
  if (fscanf(fin, "%lu %lu", &n, &m) != 2)
    printf("ERROR: couldn't load matrix sizes\n");
  initializeStructs();

  int a, b;
  nedges = 0;
  while(fscanf(fin, "%d %d", &a, &b) > 0) {
    addEdge(a, b);
  }
}

void Graph::calcDegrees() {
  maxDegree[0] = maxDegree[1] = 0;
  degrees[0] = vector<size_t>(n, 0);
  degrees[1] = vector<size_t>(m, 0);
  for (size_t i = 0; i < n; i++) {
    degrees[0][i] = vEL[i].size();
    for (size_t j = 0; j < vEL[i].size(); j++)
      degrees[1][vEL[i][j]]++;
    maxDegree[0] = max(maxDegree[0], degrees[0][i]);
  }
  for (size_t i = 0; i < m; i++)
    maxDegree[1] = max(maxDegree[1], degrees[1][i]);
}

size_t Graph::getDegree(int mode, int k) {
  return degrees[mode][k];
}

size_t Graph::getMaxDegree(int k) {
  return maxDegree[k];
}

void Graph::setAM(int a, int b, bool val) {
  AM[a*m+b] = val;
}

bool Graph::getAM(int a, int b) {
  return AM[a*m+b];
}
/*
size_t Graph::numNeighbors(int k) {
  if (hasVectorEL)
    return vEL[k].size();
  if (hasSetEL  )
    return sEL[k].size();
  puts("numNeighbors, but no vector/set EL");
  return 99999999;
}

int Graph::getNeighbor(int node, int k) {
  if (hasVectorEL)
    return vEL[node][k];
  puts("getNeighbor and no vector EL");
  return -1;  
}
*/
// Returns: m[i] = j, means that element "i" should be in position "j"
// Args: mode to sort, elems in mode 0, elems in mode 1
map<int, int> Graph::sortByDegree(const size_t mode, const set<int> elems[]) {
  if (!hasVectorEL) {
    puts("Graph::sortByDegree needs Vector Edge List to be active");
    exit(-1);
  }

  // calculate internal degrees
  map<int, int> degrees;
  for (set<int>::iterator it = elems[mode].begin(); it != elems[mode].end(); it++)
    degrees[*it] = 0;
  for (set<int>::iterator it = elems[0].begin(); it != elems[0].end(); it++) {
    for (size_t i = 0; i < vEL[*it].size(); i++) 
      if (elems[1].find(vEL[*it][i]) != elems[1].end())
	degrees[(mode == 0? *it : vEL[*it][i])]++;
  }
  
  // move to vector and sort
  vector<int> dv;
  for (map<int, int>::iterator it = degrees.begin(); it != degrees.end(); it++)
    dv.push_back(it->second);
  sort(dv.rbegin(), dv.rend());

  // get new positions
  map<int, int> pos;
  for (set<int>::iterator it = elems[mode].begin(); it != elems[mode].end(); it++) 
    for (size_t i = 0; i < dv.size(); i++) 
      if (degrees[*it] == dv[i]) {
	pos[*it] = i;
	dv[i] = -1;
	break;
      }
  return pos;
}

void Graph::writeSubMatrixEL(const map<int, int> elems[2], const char *filename) {
  // print EL to file
  FILE *fout = fopen(filename, "w");
  for (map<int, int>::const_iterator it = elems[0].begin(); it != elems[0].end(); it++)
    for (size_t i = 0; i < vEL[it->first].size(); i++) {
      map<int, int>::const_iterator it2 = elems[2].find(vEL[it->first][i]);
      if (it2 != elems[2].end()) 
	fprintf(fout, "%d %d\n", it->second, it2->second);
    }
  fclose(fout);
}

// calculate log(1+e^x)
double log1pex(double x) {
  double ex = exp(x);
  if (std::isinf(ex))
    return x;
  return log(1+ex);
}
