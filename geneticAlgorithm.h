#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

static int mutationRate;
static long iterations;
#endif

#include "data.h"
#include "Iteration.h"

static double findOptimalIndividualSimilarityMeasure(data *);
static double findOptimalGeneSimilarityMeasure(data *);
static data * copyData(data *);
static Iteration * reproduce(Iteration **, int, data *);
static Iteration ** createNewPopulation(Iteration **, int, data *);
static Iteration ** initializePopulation(data *, int);
void geneticAlgorithm(long, data *, int, int);
