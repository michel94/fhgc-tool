#include "ApproxGradient.h"

#define VERBOSE 0

ApproxGradient::ApproxGradient(Graph *_g, size_t factors, bool _equiv, double _r, int _samples, double _pratio):g(_g), equiv(_equiv), r(_r), f(factors), samples(_samples), pratio(_pratio) {
  cur = new Decomposition(g, factors, r, equiv);
  oth = new Decomposition(g, factors, r, equiv);
  
  cEdges = new set<pair<size_t, size_t> >[factors];
}

double ApproxGradient::getR() {
  return r;
}

void ApproxGradient::delRef(pair<size_t, size_t> k) {
  map<pair<size_t, size_t>, int>::iterator it = pEdges.find(k);
  if (it->second == 1)
    pEdges.erase(it);
  else
    it->second--;
}

void ApproxGradient::addRef(pair<size_t, size_t> k) {
  map<pair<size_t, size_t>, int>::iterator it = pEdges.find(k);
  if (it == pEdges.end()) {
    pEdges[k] = 1;
  }
  else
    it->second++;
}

void ApproxGradient::predictEdges(Decomposition *d, int factor) {
  for (set<pair<size_t, size_t> >::iterator it = cEdges[factor].begin(); it != cEdges[factor].end(); it++)
    delRef(*it);
  cEdges[factor].clear();
  
  vector<pair<double, size_t> > af;
  for (size_t i = 0; i < d->A[factor].size(); i++)
    af.push_back(make_pair(d->A[factor][i], i));
  sort(af.rbegin(), af.rend());
  
  vector<pair<double, size_t> > bf;
  if (!equiv) {
    for (size_t i = 0; i < d->B[factor].size(); i++)
      bf.push_back(make_pair(d->B[factor][i], i));
    sort(bf.rbegin(), bf.rend());
  }
  vector<pair<double, size_t> >& v2 = (equiv?af:bf);

  size_t* last = new size_t[g->getN()+10];
  memset(last, 0, sizeof(size_t)*g->getN());
  priority_queue<pair<double, size_t> > pq;
  pq.push(make_pair(af[0].first*v2[0].first, 0));

  //  puts("- Choosing edges");
  for (size_t i = 0; i < pratio*g->getNEdges() && !pq.empty(); i++) {
    pair<double, size_t> top = pq.top(); pq.pop();
    if (top.first < r/f) break;
    size_t row = top.second, col = last[row];
    pair<size_t, size_t> toInsert = make_pair(af[row].second, v2[col].second);
    cEdges[factor].insert(toInsert);
    addRef(toInsert);

    last[row]++;
    // add same row (if possible)
    if (row == 0 && last[row] < g->getM())
      pq.push(make_pair(af[0].first*v2[last[row]].first, row));
    else if (last[row-1] > last[row] && last[row] < g->getM())
      pq.push(make_pair(af[row].first*v2[last[row]].first, row));
    if ((last[row+1] == last[row]-1) && (row+1 < g->getN()) && (last[row+1] < g->getM()))
      pq.push(make_pair(af[row+1].first*v2[last[row+1]].first, row+1));
  }
  delete [] last;
}

void ApproxGradient::calcGradient(Decomposition *oth, int f1, double gradA[], double gradB[]) {
  double expr = exp(r);
  memset(gradA, 0, sizeof(double)*g->getN());
  if (!equiv) memset(gradB, 0, sizeof(double)*g->getM());

  // add real edges
  for (size_t i = 0; i < g->getN(); i++) {
    for (size_t j = 0; j < g->numNeighbors(i); j++) {
      int dest = g->getNeighbor(i, j);
      gradA[i] -= oth->B[f1][dest];
      if (!equiv) gradB[dest] -= oth->A[f1][i]; 
    }
  }
  
  // add sum over all j, considering Sij = 0
  double Asumbjk = 0;
  for (size_t i = 0; i < g->getM(); i++)
    Asumbjk += oth->B[f1][i];
  Asumbjk /= (1+expr);
  if (equiv) Asumbjk = 2*Asumbjk; // we need to consider (i,j) and (j,i)
  for (size_t i = 0; i < g->getN(); i++) {
    gradA[i] += Asumbjk;
    if (equiv) gradA[i] -= oth->A[f1][i]/(1+expr); // we considered (i,i) twice in the AA^T case
  }
  
  double Bsumbjk = 0;
  if (!equiv) {
    for (size_t i = 0; i < g->getN(); i++)
      Bsumbjk += oth->A[f1][i];
    Bsumbjk /= (1+expr);
    for (size_t i = 0; i < g->getM(); i++)
      gradB[i] += Bsumbjk;
  }
  
  // subtract predicted edges
  for (map<pair<size_t, size_t>, int>::iterator it = pEdges.begin(); it != pEdges.end(); it++) {
    size_t source = it->first.first, dest = it->first.second;
    double sum = 0;
    for (size_t f2 = 0; f2 < f; f2++)
      sum += oth->A[f2][source]*oth->B[f2][dest];
    gradA[source] += oth->B[f1][dest]/(1+exp(r-sum));

    if (!equiv) gradB[dest] += oth->A[f1][source]/(1+exp(r-sum));
    
    // fix the assumption that Sij = 0
    gradA[source] -= oth->B[f1][dest]/(1+expr);
    if (!equiv) gradB[dest] -= oth->A[f1][source]/(1+expr);
  }
}

Decomposition* ApproxGradient::calc(size_t iter, double absstop, double relstop) {
  const double projection = 1e-5;

  gettimeofday(&cur->t1, NULL);
  // init
  srand(0);
  
  // Uncomment the following lines for different initializations
  //g->calcDegrees();
  //oth->initToDegrees();
  oth->initRandomize(projection, 0.01);
  double *gradA = new double[g->getN()];
  double *gradB = new double[g->getM()];
  
  double step[f];
  for (size_t i = 0; i < f; i++)
    step[i] = pow(2,-ceil(log(g->getN())));

  // get most important edges given this initialization
  //predictEdges(oth, 0, pEdges, cEdges[0]);

  for (size_t f1 = 0; f1 < f; f1++)
    predictEdges(oth, f1);

  double oldfin = calcFpredicted(oth);
  double oldfout = calcFsampling(oth, samples);
  double sampf = oldfout;
  if(VERBOSE)
    printf("Initial predF: %lf sampF: %lf\n", oldfin, oldfout);
  
  size_t t = 1;
  do {
    oldfout = sampf;
    bool improvement = false;

    for (size_t f1 = 0; f1 < f; f1++) {
      calcGradient(oth, f1, gradA, gradB);
      
      // Step down this gradient until we improve
      size_t tries = 0;
      double newf;
      
      do {
        if (tries) step[f1] /= 2;

        for (size_t i = 0; i < g->getN(); i++) {
          cur->A[f1][i] = oth->A[f1][i]-step[f1]*gradA[i];
          if (cur->A[f1][i] < 1e-5)
            cur->A[f1][i] = 1e-5;
        }
        if (!equiv) 
          for (size_t i = 0; i < g->getM(); i++) {
            cur->B[f1][i] = oth->B[f1][i]-step[f1]*gradB[i];
            if (cur->B[f1][i] < 1e-5) {
              cur->B[f1][i] = 1e-5;
            }
          }
        
        /* Goal: force this factor to "live" with its choices  */
        // remove edges from previous iteration
        //printf("Predicting\n");
        predictEdges(cur, f1);
        //printf("Calculating F predicted\n");
        newf = calcFpredicted(cur);
        if(VERBOSE)
          printf("-- Factor %lu , iteration %lu. F = %.8lf (step = %e)\n", f1, tries, newf, step[f1]);
      } while (newf > oldfin && ++tries < iter);
    
      if (newf <= oldfin) {
        oldfin = newf;
        step[f1] *= 2;
        oth->A[f1] = cur->A[f1];
        if (!equiv)
          oth->B[f1] = cur->B[f1];
        improvement = true;
      }
      else {
        // don't completely destroy the step..
        step[f1] *= pow(2,iter-2);
        // fix the predicted edges
        predictEdges(cur, f1);
      }
    }
    if (!improvement) {
      puts("Stopped due to no improvement.");
      break;
    }

    sampf = calcFsampling(cur, samples);

    if(VERBOSE)
      printf("Iteration %lu -- oldF = %lf , newF = %lf , ABSOLUTE DIFFERENCE = %lf , RELATIVE DIFFERENCE = %lf\n", t, oldfout, sampf, (oldfout-sampf), (oldfout-sampf)/oldfout);
    
    //cur->save("communities/", "labels-");
  } while ((t++ < iter && (oldfout-sampf) > 0) || ((oldfout-sampf) > absstop) || ((oldfout-sampf)/oldfout > relstop));
  
  delete [] gradA;
  delete [] gradB;

  return cur;
}

double ApproxGradient::calcFpredicted(Decomposition *d) {
  double loger = log(1+exp(-r));
  double val = g->getN()*g->getN()*loger;
  
  // fix initial approximation error
  val -= g->getNEdges()*loger;
  // Add graph edges
  for (size_t i = 0; i < g->getN(); i++) {
    for (size_t j = 0; j < g->numNeighbors(i); j++) {
      int dest = g->getNeighbor(i, j);
      if (pEdges.find(make_pair(i, dest)) == pEdges.end()) {
        double sum = 0;
        for (size_t f1 = 0; f1 < f; f1++)
          sum += d->A[f1][i] * d->B[f1][dest];
        val += log1pex(r-sum);
      }
      else // put back the initial approximation as the prediction will also try to fix it
        val += loger; 
    }
  }

  
  // fix predicted edges error
  val -= pEdges.size()*loger;
  // add predicted edges
  size_t i = 0;
  for (map<pair<size_t, size_t>, int>::iterator it = pEdges.begin(); it != pEdges.end(); it++, i++) {
    size_t src = it->first.first;
    size_t dst = it->first.second;
    double sum=0;
    for (size_t f1 = 0; f1 < f; f1++)
      sum += d->A[f1][src] * d->B[f1][dst];
    if (g->hasEdge(src, dst))
      val += log1pex(r-sum);
    else
      val += log1pex(sum-r);
  }
  return val;
}

double ApproxGradient::calcFsimulate(Decomposition* d, int nsamples) {
  double val = 0;
  double r = d->r;
  Graph &g = *(d->g);
  
  if (g.getN()*g.getN() <= g.getNEdges()) return 0;

  for (int i = 0; i < nsamples; i++) {
    int a = rand()%g.getN(), b;
    if (d->equiv)
      b = rand() % g.getN();
    else
      b = rand() % g.getM();
    
    if (g.hasEdge(a, b)) { // only sample zeros
      i--;
      continue;
    }

    double sum = d->score(a, b);
    if (g.hasEdge(a, b))
      val += log1pex(r-sum);
    else
      val += log1pex(sum-r);
  }
  return (g.getN()*g.getM()-g.getNEdges())*1.0/nsamples*val;
}

double ApproxGradient::calcFsampling(Decomposition* d, int nsamples) {
  Graph &g = *(d->g);
  double r = d->r;
  double onesscore = 0;
  for (size_t i = 0; i < g.getN(); i++) 
    for (size_t j = 0; j < g.numNeighbors(i); j++)
      onesscore += log1pex(r-d->score(i, g.getNeighbor(i, j)));
  
  vector<double> samples;
  for (int i = 0; i < 9; i++)
    samples.push_back(calcFsimulate(d, nsamples));

  nth_element(samples.begin(), samples.begin()+4, samples.end());
  
  //printf("1score = %lf 0score = %lf\n", onesscore, samples[4]);
  return onesscore + samples[4];
}
