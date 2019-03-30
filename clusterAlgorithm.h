#ifndef CLUSTERALGORITHM_H
#define CLUSTERALGORITHM_H
#endif

#include "data.h"
#include "Iteration.h"

static void moveIndividualToCentroid(data *, int, int);
double findSimilarityMeasure(data *, double *, double *);
void clusterIndividuals(Iteration *);
