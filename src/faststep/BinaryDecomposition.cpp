#include "BinaryDecomposition.h"

vector<vector<double> >& checkEquiv(vector<vector<double> >& A, bool equiv) {
  if (equiv)
    return A;
  else
    return *(new vector<vector<double> >());
}

Decomposition::Decomposition(Graph *_g, int _f, double _r, bool _equiv):B(checkEquiv(A, _equiv)) {
  f = _f;
  r = _r;
  g = _g;
  equiv = _equiv;
  A.reserve(f);
  for (int i = 0; i < f; i++)
    A.push_back(vector<double>(g->getN(),0));
  if (!equiv) {
    B.reserve(f);  
    for (int i = 0; i < f; i++)
      B.push_back(vector<double>(g->getM(),0));
  }
}

void Decomposition::initToDegrees() {
  for (int i = 0; i < f; i++)
    for (size_t j = 0; j < g->getN(); j++)
      A[i][j] = double(g->getDegree(0, j))/g->getMaxDegree(0) + double(rand())/RAND_MAX*0.01;
  if (!equiv) {
    for (int i = 0; i < f; i++)
      for (size_t j = 0; j < g->getM(); j++)
        B[i][j] = double(g->getDegree(1, j))/g->getMaxDegree(1) + double(rand())/RAND_MAX*0.01;
  }
}

void Decomposition::initEpsilon(double eps) {
  for (int i = 0; i < f; i++)
    for (size_t j = 0; j < g->getN(); j++)
      A[i][j] = eps;
  if (!equiv) {
    for (int i = 0; i < f; i++)
      for (size_t j = 0; j < g->getM(); j++)
        B[i][j] = eps;
  }
}

void Decomposition::initRandomize(double min, double max) {
  for (int i = 0; i < f; i++)
    for (size_t j = 0; j < g->getN(); j++)
      A[i][j] = min+double(rand())/RAND_MAX*(max-min);
  if (!equiv) {
    for (int i = 0; i < f; i++)
      for (size_t j = 0; j < g->getM(); j++)
        B[i][j] = min+double(rand())/RAND_MAX*(max-min);
  }
}

void Decomposition::load(const char folder[256]) {
  FILE *fin;
  for (int i = 0; i < f; i++) {
    char filename[256];
    sprintf(filename, "%scom%d_0.txt" ,folder, i);
    fin = fopen(filename, "r");
    for (size_t j = 0; j < g->getN(); j++)
      if (fscanf(fin, "%lf", &A[i][j]) != 1)
        printf("ERROR: couldn't load factor %d, value %lu (A matrix)\n", i, j);
    fclose(fin);
    if (!equiv) {
      sprintf(filename, "%s/com%d_1.txt" ,folder, i);
      fin = fopen(filename, "r");
      for (size_t j = 0; j < g->getM(); j++)
        if (fscanf(fin, "%lf", &B[i][j]) != 1)
          printf("ERROR: couldn't load factor %d, value %lu (B matrix)\n", i, j);;
      fclose(fin);
    }
  }
}

double maxBk(Decomposition* d, int k, int& index){
  double m = -1.0;
  for(int i=0; i<(int) d->g->getN(); i++){
    if(d->A[k][i] > m){
      m = d->A[k][i];
      index = i;
    }
  }
  return m;
}

double factorAt(Decomposition *d, int j, int i){
  return d->A[i][j];
}

int highestFactor(Decomposition *d, int j){
  int mi = 0;
  double mx = -1;
  for(int i=0; i<d->f; i++){
    if(d->A[i][j] > mx){
      mx = d->A[i][j];
      mi = i;
    }
  }
  
  return mi;
}

void Decomposition::save(const char folder[256], const char labelsPrefix[256]) {
  // saving
  for (int i = 0; i < f; i++) {
    char filename[200];
    sprintf(filename, "%scom%d_0.txt", folder, i);
    //printf("Saving %s\n", filename);
    FILE *fout = fopen(filename, "w");
    if (fout == 0) {
      printf("ERROR: Could't write to %s.\n", filename);
      exit(-1);
    }
    for (size_t j = 0; j < g->getN(); j++)
      fprintf(fout, "%lf\n", A[i][j]);
    fclose(fout);
    if (!equiv) {
      sprintf(filename, "%scom%d_1.txt", folder, i);
      //printf("Saving %s\n", filename);
      FILE *fout = fopen(filename, "w");
      for (size_t j = 0; j < g->getM(); j++)
        fprintf(fout, "%lf\n", B[i][j]);
      fclose(fout);
    }
  }

  char labelsfile[128];
  sprintf(labelsfile, "%s%s.txt", folder, labelsPrefix);
  FILE *fout = fopen(labelsfile, "w");
  count.resize(f);
  fill(count.begin(), count.end(), 0);
  for (size_t j = 0; j < g->getM(); j++){
    int hf = highestFactor(this, j);
    count[hf]++;
    fprintf(fout, "%d\n",  hf);
  }
  fclose(fout);

  char filename[256];
  gettimeofday(&t2, NULL);

  int hours, minutes, seconds;
  seconds = (t2.tv_sec - t1.tv_sec);
  seconds += (t2.tv_usec - t1.tv_usec) / 1000000.0;

  sprintf(filename, "log-%d.txt", f);
  FILE* statsFile = fopen(filename, "w");
  hours = seconds / (60*60);
  seconds -= hours * (60*60);
  minutes = seconds / 60;
  seconds -= minutes * 60;
  fprintf(statsFile, "Running with %d factors.\nElapsed time: %d:%d:%d \n", f, hours, minutes, seconds);
  fprintf(statsFile, "Community sizes: ");
  for(int i=0; i<f; i++){
    fprintf(statsFile, "%d ", count[i]);
  }
  fprintf(statsFile, "\n");
  fclose(statsFile);
}

bool Decomposition::predicted(int i, int j, double r) {
  return score(i, j) >= r;
}

double Decomposition::score(int i, int j) {
  double sum = 0;
  for (int k = 0; k < f; k++) {
    if (equiv)
      sum += A[k][i]*A[k][j];
    else
      sum += A[k][i]*B[k][j];
  }
  return sum;
}

double BinaryDecomposition::calcF(Decomposition* d) {
  double val = 0;
  Graph &g = *(d->g);
  double r = d->r;
  size_t n = g.getN(), m = (d->equiv?g.getN():g.getM());
  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < m; j++) {
      double sum = d->score(i,j);
      double newval;
      if (g.hasEdge(i,j))
        newval = log1pex(-1*(sum-r));
      else 
        newval = log1pex(sum-r);
      val += newval;
    }
    //putchar('\n');
  }
  return val;
}
