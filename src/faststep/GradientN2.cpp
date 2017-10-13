#include "GradientN2.h"

GradientN2::GradientN2(Graph *_g, int factors, bool _equiv, double _r, double lambda):g(_g), equiv(_equiv), r(_r), l(lambda), f(factors) {
  cur = new Decomposition(g, factors, r, equiv);
  oth = new Decomposition(g, factors, r, equiv);
}

GradientN2::~GradientN2() {
  delete cur;
  delete oth;
}

double GradientN2::getR() {
  return r;
}

void GradientN2::calcGradient(Decomposition *oth, int f1, double gradA[], double gradB[]) {
  // Calculate gradients of A for this factor
  memset(gradA, 0, sizeof(double)*g->getN());
  for (size_t i = 0; i < g->getN(); i++) {
    for (size_t j = 0; j < g->getM(); j++) {
      if (equiv) { // we are decomposing into AA^T
	if (g->hasEdge(i, j)) 
	  gradA[i] -= l*oth->A[f1][j]/(1+exp(sum[i][j]-r));
	else
	  gradA[i] += oth->A[f1][j]/(1 + exp(r-sum[i][j]));
	// update j's gradient
	if (g->hasEdge(j, i)) 
	  gradA[j] -= l*oth->A[f1][i]/(1+exp(sum[j][i]-r));
	else
	  gradA[j] += oth->A[f1][i]/(1 + exp(r-sum[j][i]));
      }
      else { // into AB^T
	if (g->hasEdge(i, j)) 
	  gradA[i] -= l*oth->B[f1][j]/(1+exp(sum[i][j]-r));
	else
	  gradA[i] += oth->B[f1][j]/(1 + exp(r-sum[i][j]));
      }
    }
  }

  // Calculate gradients of B for this factor
  if (!equiv) {
    memset(gradB, 0, sizeof(double)*g->getM());
    for (size_t i = 0; i < g->getN(); i++) {
      for (size_t j = 0; j < g->getM(); j++) {
	if (g->hasEdge(i, j)) 
	  gradB[j] -= l*oth->A[f1][i]/(1+exp(sum[i][j]-r));
	else
	  gradB[j] += oth->A[f1][i]/(1 + exp(r-sum[i][j]));
      }
    }
  }
}

void GradientN2::calcSum(Decomposition *d) {
  for (size_t i = 0; i < g->getN(); i++)
    for (size_t j = 0; j < g->getM(); j++) {
      sum[i][j] = 0;
      for (int f1 = 0; f1 < f; f1++) {
	if (equiv)
	  sum[i][j] += d->A[f1][i]*d->A[f1][j];
	else
	  sum[i][j] += d->A[f1][i]*d->B[f1][j];
      }
    }
}

Decomposition* GradientN2::calc(size_t iter, double absstop, double relstop) {
  const double projection = 1e-5;
  // init
  srand(0);
  //g->calcDegrees();
  //oth->initToDegrees();
  //oth->initEpsilon(0.01);
  oth->initRandomize(projection, 0.01);

  double gradA[g->getN()];
  double gradB[g->getM()];
  
  double step[f];
  for (int i = 0; i < f; i++)
    step[i] = 1; //pow(2,-ceil(log(max(g->getN(), g->getM())))); //pow(2,-6);

  double oldf = calcF(oth);
  double olditerf = oldf;
  double curf = oldf;

  printf("Initial score: %lf\n", oldf);

  calcSum(oth);
  
  size_t t = 1;
  do {
    olditerf = curf;
    for (int f1 = 0; f1 < f; f1++) {
      calcGradient(oth, f1, gradA, gradB);
      
      // Step down this gradient until we improve
      int sent0;
      size_t tries = 0;
      do {
	if (tries) step[f1] /= 2;
	sent0 = 0;

	for (size_t i = 0; i < g->getN(); i++) {
	  cur->A[f1][i] = oth->A[f1][i]-step[f1]*gradA[i];
	  if (cur->A[f1][i] < 1e-5) {
	    cur->A[f1][i] = 1e-5;
	    sent0++;
	  }
	}
	if (!equiv) 
	  for (size_t i = 0; i < g->getM(); i++) {
	    cur->B[f1][i] = oth->B[f1][i]-step[f1]*gradB[i];
	    if (cur->B[f1][i] < 1e-5) {
	      cur->B[f1][i] = 1e-5;
	      sent0++;
	    }
	  }

	curf = calcF(cur);
	printf("-- Factor %d , iteration %lu. F = %.8lf (step = %e)\n", f1, tries, curf, step[f1]);
      } while (curf > oldf && tries++ < 10);
    
      if (curf <= oldf) {
	/*
	puts("-- IMPROVED Iteration");
	for (size_t i = 0; i < g->getN(); i++)
	  printf("%lf (%lf) ", oth->x[f1][i], cur->x[f1][i]);
	putchar('\n');
	*/
	step[f1] *= 2;
	oldf = curf;
	
	for (size_t i = 0; i < g->getN(); i++)
	  for (size_t j = 0; j < g->getM(); j++) {
	    if (equiv) {
	      sum[i][j] -= oth->A[f1][i]*oth->A[f1][j];
	      sum[i][j] += cur->A[f1][i]*cur->A[f1][j];
	    }
	    else {
	      sum[i][j] -= oth->A[f1][i]*oth->B[f1][j];
	      sum[i][j] += cur->A[f1][i]*cur->B[f1][j];
	    }
	  }
	
	oth->A[f1] = cur->A[f1];
	if (!equiv)
	  oth->B[f1] = cur->B[f1];
      }
      /*else {
	puts("Can't improve on this gradient:");
	for (size_t i = 0; i < g->getN(); i++)
	  printf("%lf (%lf) ", oth->x[f1][i], grad[i]);
	putchar('\n');
	}*/
    }
    printf("Iteration %lu -- oldF = %lf , newF = %lf , ABSOLUTE DIFFERENCE = %lf , RELATIVE DIFFERENCE = %lf\n", t, olditerf, curf, (olditerf-curf), (olditerf-curf)/olditerf);
  } while (t++ < iter || (olditerf-curf > absstop) || ((olditerf-curf)/olditerf > relstop));
  
  return cur;
}
