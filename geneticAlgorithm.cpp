#include <iostream>
#include <cstdlib>
#include <ctime>

#include "data.h"
#include "clusterAlgorithm.h"
#include "Iteration.h"

using namespace std;

int mutationRate;

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

	offspring->setFitness(rand() % 100);	

	return offspring;

}

static Iteration ** createNewPopulation(Iteration ** oldPopulation, int populationSize, data * dataset) {

	Iteration ** newPopulation = new Iteration*[populationSize];
	for (int i = 0; i < populationSize; i++)
		newPopulation[i] = reproduce(oldPopulation, populationSize, dataset);

	delete[] oldPopulation;
	return newPopulation;
	

}
 
void geneticAlgorithm(long iterations, data * dataSet, int populationSize, int chanceOfMutation) {

	srand(time(NULL));	
	mutationRate = chanceOfMutation;

}
