#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

#include "data.h"
#include "clusterAlgorithm.h"
#include "generateBMP.h"
#include "Iteration.h"

using namespace std;

int mutationRate;
long iterations = 0;


//TODO: figure out how to find optimalSimilarityMeasures
static double findOptimalIndividualSimilarityMeasure(data * d) {
	double x = 5;
	return x;
}
static double findOptimalGeneSimilarityMeasure(data * d) {
	double x = 5;
	return x;
}
	
//copies data set and returns copy
static data * copyData(data * d) {

	int i,j;
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
	
	long totalFitness = 0;
	
	for (i = 0; i < populationSize; i++) {
		if ((oldPopulation[i]->getFitness() < 100) && (oldPopulation[i]->getFitness() >= 0))
			totalFitness += (oldPopulation[i]->getFitness());
		else
			cout << "Error, fitness is value not between 0-99\n";
	}
	
	

	//holds copies of iteration pointers for picking parents
	//each iteration will have multiple copies in the pool,
	//the higher the fitness, the more copies
	//parents are chosen by picking random indices
	Iteration * naturalSelectionPool[totalFitness];
		
	long count = 0;
	for (i = 0; i < populationSize; i++) {
		if (count >= totalFitness) {
			//cout << "Error generating natural selection pool." << count << " : " << totalFitness << endl;	
			break;
		}
		else
			for (j = 0; j < (oldPopulation[i]->getFitness()); j++) {
				naturalSelectionPool[count] = oldPopulation[i];
				count++;
			}
	}
	

	//change this so offspring inherits "genes" from different amount of parents
	int parentCount = 2;

	//get parents from emulated natural selection
	Iteration * parents[parentCount];

	for (i = 0; i < parentCount; i++)
		parents[i] = naturalSelectionPool[rand() % totalFitness];
	
	//create new Iteration object
	Iteration * offSpring = new Iteration(copyData(dataset));	
	
	//
	//
	//set amount of clusters for new offspring
	//
	//

	if ((rand() % mutationRate) == 0) { //chance of mutation
		offSpring->setKForIndividuals(rand() % (dataset->numIndividuals * 1/4 + 1) + 1);
	}
	else {//otherwise inherits from parents
		//get k from random parent
		offSpring->setKForIndividuals(parents[rand() % parentCount]->getKForIndividuals());
	}

	if ((rand() % mutationRate) == 0)
		offSpring->setKForGenes(rand() % (dataset->numGenes * 1/4 + 1) + 1);
	else
		offSpring->setKForGenes(parents[rand() % parentCount]->getKForGenes());

	//
	//
	//getting centroids from parents
	//
	//TODO: Currently needs work. Cannot have duplicate centroid indices.

 	int * individualCentroids = new int[offSpring->getKForIndividuals()];
	int * geneCentroids = new int[offSpring->getKForGenes()];
	
	for (i = 0; i < offSpring->getKForIndividuals(); i++) {
		if ((rand() % mutationRate) == 0) { //inherit mutated centroid (just random centroid)
			individualCentroids[i] = rand() % dataset->numIndividuals;
		}
		else { //inherit centroids from parents			
			int parent = rand() % parentCount;
			individualCentroids[i] = parents[parent]->getIndividualCentroids()[rand() % (parents[parent]->getKForIndividuals())];
		}
	}
	for (i = 0; i < offSpring->getKForGenes(); i++) {
		if ((rand() % mutationRate) == 0)
			geneCentroids[i] = rand() % dataset->numGenes;
		else {
			int parent = rand() % parentCount;
			geneCentroids[i] = parents[parent]->getGeneCentroids()[rand() % (parents[parent]->getKForGenes())];
		}
	}
	

	offSpring->setIndividualCentroids(individualCentroids);
	offSpring->setGeneCentroids(geneCentroids);
	offSpring->setIndividualSimilarityMeasure(findOptimalIndividualSimilarityMeasure(dataset));
	offSpring->setGeneSimilarityMeasure(findOptimalGeneSimilarityMeasure(dataset));
	offSpring->setFitness(rand() % 100);

	clusterIndividuals(offSpring);
	clusterGenes(offSpring);	

	offSpring->print();

	return offSpring;

}

//create new population of given amount of iterations
static Iteration ** createNewPopulation(Iteration ** oldPopulation, int populationSize, data * dataset) {

	Iteration ** newPopulation = new Iteration*[populationSize];
	for (int i = 0; i < populationSize; i++)
		newPopulation[i] = reproduce(oldPopulation, populationSize, dataset);
	
	
	//Generate bitmap heatmaps
	/*	
	for (int i = 0; i < populationSize; i++) {
		if ((iterations % 10) == 0) {
			string filename = "../results/";
			filename += ("pop" + to_string(iterations) + "iteration" + to_string(i) + ".bmp");
			generateBMP(newPopulation[i]->getData(), filename);
		}
		
	}
	*/

	iterations++;
	cout << "New population generated\n";
	
	delete[] oldPopulation;

	return newPopulation;	

}

//create initial population of iterations 
static Iteration ** initializePopulation(data * dataSet, int populationSize) {
	
	Iteration ** newPopulation = new Iteration*[populationSize];
	
	for (int i = 0; i < populationSize; i++) {
	
		newPopulation[i] = new Iteration(copyData(dataSet));
		newPopulation[i]->setKForIndividuals(5);		//rand() % (dataSet->numIndividuals - (1/4*(dataSet->numIndividuals))) + 5);
		newPopulation[i]->setKForGenes(5);
		newPopulation[i]->setIndividualCentroids(new int[newPopulation[i]->getKForIndividuals()]);
		newPopulation[i]->setGeneCentroids(new int[newPopulation[i]->getKForGenes()]);
		newPopulation[i]->setIndividualSimilarityMeasure(5);
		newPopulation[i]->setGeneSimilarityMeasure(5);
		
		//set centroids to random individuals
	
		vector<int> usedNumbers;

		for (int j = 0; j < (newPopulation[i]->getKForIndividuals()); j++) {
			bool isDuplicate = false;
			int centroid = rand() % dataSet->numIndividuals;
			for (int k = 0; k < usedNumbers.size(); k++) 
				if (centroid == usedNumbers[k]) {
					isDuplicate = true;
					break;
				}
		
			if (!isDuplicate) {
				newPopulation[i]->getIndividualCentroids()[j] = centroid;
				usedNumbers.push_back(centroid);
			}
			else
				j--;
		}

		//set centroids to random genes

		usedNumbers.clear();
		for (int j = 0; j < (newPopulation[i]->getKForGenes()); j++) {
			bool isDuplicate = false;
			int centroid = rand() % dataSet->numGenes;
			for (int k = 0; k < usedNumbers.size(); k++)
				if (centroid == usedNumbers[k]) {
					isDuplicate = true;
					break;
				}
			if (!isDuplicate) {
				newPopulation[i]->getGeneCentroids()[j] = centroid;
				usedNumbers.push_back(centroid);
			}
			else
				j--;
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
	
			
	for (long i = 0; i < iterations; i++) {
		
		population = createNewPopulation(population, populationSize, dataSet);

	}
	
	

	/*	
	for (int i = 0; i < populationSize; i++)
		delete population[i];	
	*/

	delete[] population;
	
}
