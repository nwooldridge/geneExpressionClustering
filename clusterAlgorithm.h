#ifndef CLUSTERALGORITHM_H
#define CLUSTERALGORITHM_H
#endif

#include "data.h"
static void moveIndividualToCentroid(data *, long, long);
static double findSimilarityMeasure(data *, double *, double *);
void clusterIndividuals(data *, long, long *, double);
