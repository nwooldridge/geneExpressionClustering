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

//generates child iteration
static Iteration * reproduce(Iteration ** oldPopulation, int populationSize, data * dataset) {
	
	int i,j;
	
	//find sum of fitness of each member in population
	double fitnessSum = 0;
	for (i = 0; i < populationSize; i++)
		fitnessSum += oldPopulation[i]->getFitness();
	
	//create naturalSelectionPool which holds pointers to iterations. 
	//Used to choose parents for reproduction process
	//The higher the fitness of the Iteration, the more copies of that iteration reference in naturalSelectionPool
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
	
	//get parents of child
	long parents[2];
	parents[0] = rand() % naturalSelectionPoolSize;
	parents[1] = rand() % naturalSelectionPoolSize;
	while (naturalSelectionPool[parents[0]] == naturalSelectionPool[parents[1]])
		parents[1] = rand() % naturalSelectionPoolSize;
	
	
	//set child's K value from parent's values
	Iteration * offspring = new Iteration(copyData(dataset));
	if ((rand() % 2) == 0)
		offspring->setK(naturalSelectionPool[parents[0]]->getK());
	else
		offspring->setK(naturalSelectionPool[parents[1]]->getK());
		
	
	offspring->setCentroids(new long[offspring->getK()]);

	//get centroids 
	for (i = 0; i < offspring->getK(); i++) {
		
		//50% chance of receiving centroid from 1 parent		
		if ((rand() % 2) == 0) {
			int randInt = rand() % naturalSelectionPool[parents[0]]->getK();
			for (j = 0; j < i; j++) {
				if ((offspring->getCentroids())[j] == randInt) {
					i--;
					break;
				} 	
				else {
					(offspring->getCentroids())[i] = randInt;
				}	
			}
		}	
	 	
		//50% chance of receiving centroid from other parent
		else {
			
			int randInt = rand() % naturalSelectionPool[parents[1]]->getK();
			for (j = 0; j < i; j++) {
				 if ((offspring->getCentroids())[j] == randInt) {
                                        i--;
                                        break;
                                }
                                else {
                                        (offspring->getCentroids())[i] = randInt;
                                }
			}
		}
	}

	return offspring;

}

static Iteration ** createNewPopulation(Iteration ** oldPopulation, int populationSize, data * dataSet) {

	Iteration ** newPopulation = new Iteration*[populationSize];
	delete[] oldPopulation;
	return newPopulation;
	

} 
void geneticAlgorithm(long iterations, data * dataSet, int populationSize, int chanceOfMutation) {

	srand(time(NULL));	

}
