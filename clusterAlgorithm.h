#ifndef CLUSTERALGORITHM_H
#define CLUSTERALGORITHM_H
#endif

#include "data.h"
static void moveIndividualToCentroid(data *, int, int);
double findSimilarityMeasure(data *, double *, double *);
void clusterIndividuals(data *, int, int *, double);
