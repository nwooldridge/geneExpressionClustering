#include <iostream>
#include <cstdlib>
#include <ctime>

#include "data.h"
#include "clusterAlgorithm.h"
#include "Iteration.h"

using namespace std;

int mutationRate;


//TO-DO: figure out how to find optimalSimilarityMeasures
static double findOptimalIndividualSimilarityMeasure(data * d) {
	double x;
	return x;
}
static double findOptimalGeneSimilarityMeasure(data * d) {
	double x;
	return x;
}
	
//copies data set and returns copy
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
		naturalSelectionPoolSize += (fitnessSum / oldPopulation[i]->getFitness());
		//cout << (double) (fitnessSum / oldPopulation[i]->getFitness()) << endl;
		cout << oldPopulation[i]->getFitness() << " ";
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
	
	//generate amount of clusters
	//if mutation, generate value corresponding to amount of clusters bounded between 1 and 1/4th of number of individuals 
	if ((rand() % mutationRate) == 0)
		offspring->setK(rand() % (dataset->numIndividuals * (1/4) + 1) + 1);	
	
	else {
		if ((rand() % 2) == 0)
			offspring->setK(naturalSelectionPool[parents[0]]->getK());
		else
			offspring->setK(naturalSelectionPool[parents[1]]->getK());
	}	
	
	//create centroids array based on K value
	offspring->setCentroids(new long[offspring->getK()]);

	//set centroids for offspring
	for (i = 0; i < offspring->getK(); i++) {

		long centroid;

		//generate mutated centroid
		if ((rand() % mutationRate) == 0) {
			centroid = rand() % dataset->numIndividuals;
		}
		
		//else get centroid from parent
		else {
			int parent = rand() % 2;
	                int randInt = rand() % (naturalSelectionPool[parents[parent]]->getK());
			centroid = naturalSelectionPool[parents[parent]]->getCentroids()[randInt];
		
		}

		bool isExistantCentroid = false;
		for (j = 0; j < i; j++) {
			if ((offspring->getCentroids())[j] == centroid) {
				i--;
				isExistantCentroid = true;
				break;
			}
		} 
		
		if (!isExistantCentroid) {
			offspring->getCentroids()[i] = centroid;
		}
	}

	offspring->setIndividualSimilarityMeasure(findOptimalIndividualSimilarityMeasure(dataset));
	offspring->setGeneSimilarityMeasure(findOptimalGeneSimilarityMeasure(dataset));

	clusterIndividuals(dataset, offspring->getK(), offspring->getCentroids(), offspring->getIndividualSimilarityMeasure());

	offspring->setFitness(rand() % 100);	

	return offspring;

}

//create new population of given amount of iterations
static Iteration ** createNewPopulation(Iteration ** oldPopulation, int populationSize, data * dataset) {

	Iteration ** newPopulation = new Iteration*[populationSize];
	for (int i = 0; i < populationSize; i++)
		newPopulation[i] = reproduce(oldPopulation, populationSize, dataset);
	/*
	for (int i = 0; i < populationSize; i++)
		delete oldPopulation[i];
	*/
	delete[] oldPopulation;

	return newPopulation;	

}

//create initial population of iterations 
static Iteration ** initializePopulation(data * dataSet, int populationSize) {
	
	Iteration ** newPopulation = new Iteration*[populationSize];

	for (int i = 0; i < populationSize; i++) {
		newPopulation[i] = new Iteration(dataSet);
		newPopulation[i]->setK(rand() % (dataSet->numIndividuals *(1/4) + 1) + 1);
		newPopulation[i]->setCentroids(new long[newPopulation[i]->getK()]);
		
		//set centroids to random individuals
		for (int j = 0; j < newPopulation[i]->getK(); j++) {
			long centroid = rand() % dataSet->numIndividuals;
			bool isExistingCentroid = false;
			for (int k = 0; k < j; k++) {
				if (centroid == newPopulation[i]->getCentroids()[k]) {
					isExistingCentroid = true;
					i--;
					j--;
					break;
				}
			}
			if (!isExistingCentroid)
				newPopulation[i]->getCentroids()[j] = centroid;
		}
		
		newPopulation[i]->setFitness(rand() % 100);

	}
	return newPopulation;	

}

//entry point of this file
void geneticAlgorithm(long iterations, data * dataSet, int populationSize, int chanceOfMutation) {
	
	srand(time(NULL));	
	
	mutationRate = chanceOfMutation;

	Iteration ** population = initializePopulation(dataSet, populationSize);
	
	population = createNewPopulation(population, populationSize, dataSet);
	
	/*	
	for (long i = 0; i < iterations; i++) {
		
		population = createNewPopulation(population, populationSize, dataSet);

	}
	*/

	/*	
	for (int i = 0; i < populationSize; i++)
		delete population[i];	
	*/
	delete[] population;
	
}
