#ifndef CLUSTERALGORITHM_H
#define CLUSTERALGORITHM_H
#endif

#include "data.h"
#include "Iteration.h"

static void moveIndividualToCentroid(data *, int, int);
static void moveGeneToCentroid(data *, int, int);
static double findIndividualSimilarityMeasure(data *, double *, double *);
static double findGeneSimilarityMeasure(data *, double *, double *);
void clusterIndividuals(Iteration *);
void clusterGenes(Iteration *);
