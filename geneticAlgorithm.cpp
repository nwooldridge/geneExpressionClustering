#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>

#include "data.h"
#include "clusterAlgorithm.h"
#include "generateCSV.h"
#include "Iteration.h"
#include "fitnessFunction.h"

using namespace std;

int mutationRate;
long iterations = 0;
long totalIterations;
double dataStandardDeviation;

//Find standard deviation of data to better match similar data up
static double findStandardDeviation(data * d) {

	int i, j;
 
	double averageGeneExpression = 0;
        for (i = 0; i < d->numIndividuals; i++)
                for (j = 0; j < d->numGenes; j++)
                        averageGeneExpression += d->values[i][j];
        averageGeneExpression = averageGeneExpression / (d->numIndividuals * d->numGenes);

	double squaredDifferences = 0;

        for (i = 0; i < d->numIndividuals; i++)
                for (j = 0; j < d->numGenes; j++)
                        squaredDifferences += pow((d->values[i][j] - averageGeneExpression) , 2);
	squaredDifferences = squaredDifferences / (d->numIndividuals * d->numGenes);

	return sqrt(squaredDifferences);

}
//TODO: Improve finding optimal similarity measures. I still have almost 100% of 
//individuals being clustered. I think outliers are skewing the standard deviation 
//and making the data appear further apart than it is

//Optimal similarity measure determined by halving the standard deviation of data 
//and multiplying it by the amount of individuals or genes
static double findOptimalIndividualSimilarityMeasure(data * d) {

	return (d->numIndividuals * dataStandardDeviation * 1/100);
}
static double findOptimalGeneSimilarityMeasure(data * d) {
	
	return (d->numGenes * dataStandardDeviation * 1/4);
}
	
//copies data set and returns copy
static data * copyData(data * d) {

	int i,j;
	data * newCopy = new data;

	newCopy->numIndividuals = d->numIndividuals;
	newCopy->numGenes = d->numGenes;

	newCopy->values = new double*[d->numIndividuals];
	for (i = 0; i < d->numIndividuals; i++) {
		newCopy->values[i] = new double[d->numGenes];
		for (j = 0; j < d->numGenes; j++)
                	newCopy->values[i][j] = d->values[i][j];
	}

	return newCopy;
}

//generates child iteration
static void reproduce(Iteration ** oldPopulation, int populationSize, data * dataset, Iteration * offSpring) {
	
	int i,j;
	
	long totalFitness = 0;
	
	for (i = 0; i < populationSize; i++) {
		if ((oldPopulation[i]->getFitness() <= 100) && (oldPopulation[i]->getFitness() >= 0))
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

	//------------------------------//
	//				//
	//getting centroids from parents//
	//				//
	//------------------------------//

	cluster * individualCentroids = new cluster[offSpring->getKForIndividuals()];
	cluster * geneCentroids = new cluster[offSpring->getKForGenes()];
	
		
	//Keep track of used values so no duplicates are generated
	vector <int> usedValues;
	
	for (i = 0; i < offSpring->getKForIndividuals(); i++) {
		bool isDuplicate = false;
		int centroid = 0;
		if ((rand() % mutationRate) == 0) { //inherit mutated centroid (just random centroid)
			centroid = rand() % dataset->numIndividuals;
		}
		else { //inherit centroids from parents			
			int parent = rand() % parentCount;
			centroid = parents[parent]->getIndividualCentroids()[rand() % (parents[parent]->getKForIndividuals())].index;
		}
		for (j = 0; j < usedValues.size(); j++)
			if (centroid == usedValues[j]) {
				isDuplicate = true;
				break;
			}
		if (!isDuplicate) {
			individualCentroids[i].index = centroid;
			usedValues.push_back(centroid);
		}
		else
			i--;
	}
	
	//Do same thing as above except for the centroids for genes
	usedValues.clear();
	for (i = 0; i < offSpring->getKForGenes(); i++) {
		bool isDuplicate = false;
		int centroid = 0;
		if ((rand() % mutationRate) == 0)
			centroid = rand() % dataset->numGenes;
		else {
			int parent = rand() % parentCount;
			centroid = parents[parent]->getGeneCentroids()[rand() % (parents[parent]->getKForGenes())].index;
		}
		for (j = 0; j < usedValues.size(); j++)
			if (usedValues[j] == centroid) {
				isDuplicate = true;
				break;
			}
		if (!isDuplicate) {
			geneCentroids[i].index = centroid;
			usedValues.push_back(centroid);
		}	
		else
			i--;
	}

	//Initialize these values for clustering algorithm to set them
	for (i = 0; i < offSpring->getKForIndividuals(); i++) {
                individualCentroids[i].lowerBoundIndex = individualCentroids[i].index;
                individualCentroids[i].upperBoundIndex = individualCentroids[i].index;
                individualCentroids[i].clusterSize = 0;
        }
        for (i = 0; i < offSpring->getKForGenes(); i++) {
                geneCentroids[i].lowerBoundIndex = geneCentroids[i].index;
                geneCentroids[i].upperBoundIndex = geneCentroids[i].index;
                geneCentroids[i].clusterSize = 0;
        }
	
	offSpring->setIndividualCentroids(individualCentroids);
	offSpring->setGeneCentroids(geneCentroids);


	offSpring->setIndividualSimilarityMeasure(parents[0]->getIndividualSimilarityMeasure());
	offSpring->setGeneSimilarityMeasure(parents[0]->getGeneSimilarityMeasure());

	clusterIndividuals(offSpring);

	clusterGenes(offSpring);	

	//offSpring->print();


}

//create new population of given amount of iterations
static Iteration ** createNewPopulation(Iteration ** oldPopulation, int populationSize, data * dataset) {
	
	//SET FITNESS OF THE OLD POPULATION	
	findFitness(oldPopulation, populationSize);

	generateCSV(oldPopulation, populationSize, iterations);
	
	Iteration ** newPopulation = new Iteration*[populationSize];
	for (int i = 0; i < populationSize; i++)
	{	newPopulation[i] = new Iteration(copyData(dataset));
		reproduce(oldPopulation, populationSize, dataset, newPopulation[i]);
	}

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
		newPopulation[i]->setKForIndividuals(rand() % (dataSet->numIndividuals * 1/4 + 1) + 1);		//rand() % (dataSet->numIndividuals - (1/4*(dataSet->numIndividuals))) + 5);
		newPopulation[i]->setKForGenes(rand() % (dataSet->numGenes * 1/4 + 1) + 1);
		newPopulation[i]->setIndividualCentroids(new cluster[newPopulation[i]->getKForIndividuals()]);
		newPopulation[i]->setGeneCentroids(new cluster[newPopulation[i]->getKForGenes()]);
		newPopulation[i]->setIndividualSimilarityMeasure(findOptimalIndividualSimilarityMeasure(dataSet));
		newPopulation[i]->setGeneSimilarityMeasure(findOptimalGeneSimilarityMeasure(dataSet));
		newPopulation[i]->setAmountOfIndividualsClustered(rand() % 25);
		newPopulation[i]->setAmountOfGenesClustered(rand() % 25);	
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
				newPopulation[i]->getIndividualCentroids()[j].index = centroid;
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
				newPopulation[i]->getGeneCentroids()[j].index = centroid;
				usedNumbers.push_back(centroid);
			}
			else
				j--;
		}	
		
		newPopulation[i]->setFitness(1);

	}
	return newPopulation;	

}

//entry point of this file
void geneticAlgorithm(long iterations, data * dataSet, int populationSize, int chanceOfMutation) {
	
	srand(time(NULL));	
	
	mutationRate = chanceOfMutation;

	totalIterations = iterations;
	
	dataStandardDeviation = findStandardDeviation(dataSet);

	Iteration ** population = initializePopulation(dataSet, populationSize);
	
	for (long i = 0; i < iterations; i++) {
		
		population = createNewPopulation(population, populationSize, dataSet);

	}
	
	

	/*	
	for (int i = 0; i < populationSize; i++)
		delete population[i];	
	*/

	delete[] population;
	
}
