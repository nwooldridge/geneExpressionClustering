#include <iostream>
#include <cstdlib>
#include <ctime>

#include "data.h"
#include "clusterAlgorithm.h"
#include "Iteration.h"

using namespace std;

static double findOptimalIndividualSimilarityMeasure(data * d) {
	double x;
	return x;
}
static double findOptimalGeneSimilarityMeasure(data * d) {
	double x;
	return x;
}
static data * copyData(data * d) {

	long i,j;
	data * newCopy = new data;

	newCopy->numIndividuals = d->numIndividuals;
	newCopy->numGenes = d->numGenes;

	newCopy->values = new double*[d->numIndividuals];
	for (i = 0; i < d->numIndividuals; i++) 
		newCopy->values[i] = new double[d->numGenes];
	
	for (i = 0; i < d->numIndividuals; i++)
		for (j = 0; j < d->numGenes; j++)
			newCopy->values[i][j] = d->values[i][j];

	return newCopy;
}
static Iteration * reproduce(Iteration ** oldPopulation, int populationSize, data * dataset) {
	
	int i,j;
	double fitnessSum = 0;
	for (i = 0; i < populationSize; i++)
		fitnessSum += oldPopulation[i]->getFitness();
	
	
	long naturalSelectionPoolSize = 0;
	for (i = 0; i < populationSize; i++) {
		naturalSelectionPoolSize += fitnessSum / oldPopulation[i]->getFitness();
		cout << (double) (fitnessSum / oldPopulation[i]->getFitness()) << endl;
	}
	Iteration * naturalSelectionPool[naturalSelectionPoolSize];

	int count = 0;
	for (i = 0; i < populationSize; i++) {
		for (j = 0; j < oldPopulation[i]->getFitness(); j++) {
			naturalSelectionPool[count] = oldPopulation[i];
			count++;
		}
	}
	
	long parents[2];
	parents[0] = rand() % naturalSelectionPoolSize;
	parents[1] = rand() % naturalSelectionPoolSize;
	while (naturalSelectionPool[parents[0]] == naturalSelectionPool[parents[1]])
		parents[1] = rand() % naturalSelectionPoolSize;
	
	
	Iteration * offspring = new Iteration(copyData(dataset));
	if ((rand() % 2) == 0)
		offspring->k = naturalSelectionPool[parent[0]]->k;
	else
		offspring->k = naturalSelectionPool[parent[1]]->k;
		

	return offspring;

}
static Iteration ** createNewPopulation(Iteration ** oldPopulation, int populationSize, data * dataSet) {

	Iteration ** newPopulation = new Iteration*[populationSize];
	delete[] oldPopulation;
	return newPopulation;
	

} 
void geneticAlgorithm(long iterations, data * dataSet, int populationSize) {

	srand(time(NULL));	

}
