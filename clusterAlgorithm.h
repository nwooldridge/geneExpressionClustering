#ifndef CLUSTERALGORITHM_H
#define CLUSTERALGORITHM_H
#endif

#include "data.h"
#include "Iteration.h"

static void moveIndividualToCentroid(data *, cluster *, int, int);
static void moveGeneToCentroid(data *, cluster *, int, int);
static double findIndividualSimilarityMeasure(data *, double *, double *);
static double findGeneSimilarityMeasure(data *, int, int);
void clusterIndividuals(Iteration *);
void clusterGenes(Iteration *);
