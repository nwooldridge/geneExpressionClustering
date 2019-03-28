#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H
#endif

#include "data.h"
#include "Iteration.h"

static double findOptimalIndividualSimilarityMeasure(data *);
static double findOptimalGeneSimilarityMeasure(data *);
static data * copyData(data *);
static Iteration * reproduce(Iteration **, int, data *);
static Iteration ** createNewPopulation(Iteration **, int, data *);
void geneticAlgorithm(long, data *, int);

