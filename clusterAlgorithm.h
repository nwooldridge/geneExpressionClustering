#ifndef CLUSTERALGORITHM_H
#define CLUSTERALGORITHM_H
#endif

#include "data.h"
#include "Iteration.h"

static void moveIndividualToCentroid(data *, int, int);
double findIndividualSimilarityMeasure(data *, double *, double *);
double findGeneSimilarityMeasure(data *, double *, double *);
void clusterIndividuals(Iteration *);
void clusterGenes(Iteration *);
