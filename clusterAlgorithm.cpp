#include <cmath>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>

#include "data.h"
#include "Iteration.h"

using namespace std;

static void moveIndividualToCentroid(data * d, int individualIndex, int centroidIndividualIndex) {
	
	int i, j;
	
	vector<double *> beforeArr;
	vector<double *> afterArr;
	
	//cout << "before moving: " << d->values[ centroidIndividualIndex ];

	if (individualIndex > centroidIndividualIndex)
		afterArr.push_back(d->values[individualIndex]);

	for (i = 0; i < centroidIndividualIndex; i++) 
		if (i != individualIndex) 
			beforeArr.push_back(d->values[i]);
	
	for (i = centroidIndividualIndex + 1; i < d->numIndividuals; i++)
		if (i != individualIndex)
			afterArr.push_back(d->values[i]);

	if (individualIndex < centroidIndividualIndex)
		beforeArr.push_back(d->values[individualIndex]);		
	
	long count = 0;
	for (i = 0; i < beforeArr.size(); i++) {
		d->values[count] = beforeArr[i];
		count++;
	}
	count++;
	for (i = 0; i < afterArr.size(); i++) {
		d->values[count] = afterArr[i];
		count++;
	}
	
	//cout << " after moving: " << d->values[ centroidIndividualIndex ] << endl;
}

static void moveGeneToCentroid(data * d, int geneIndex, int centroidGeneIndex) {




}

static double findIndividualSimilarityMeasure(data * d, double * individual, double * centroidIndividual) {
	
	double similarityMeasure = 0;
	

	for (int i = 0; i < d->numGenes; i++) {
		similarityMeasure += abs(individual[i] - centroidIndividual[i]);

	}
	
	return similarityMeasure;

}

static double findGeneSimilarityMeasure(data * d, int geneIndex, int centroidGeneIndex) {

	
	double similarityMeasure = 0;

	for (int i = 0; i < (d->numIndividuals); i++) 
		similarityMeasure += abs((d->values[i][geneIndex]) - (d->values[i][centroidGeneIndex]));
	
	
		
	return similarityMeasure;
}

void clusterIndividuals(Iteration * iteration) {

	int i;

	data * d = iteration->getData();
	int k = iteration->getKForIndividuals();
	int * centroids = iteration->getIndividualCentroids();
	double minSimilarityMeasure = iteration->getIndividualSimilarityMeasure();
		
	for (i = 0; i < d->numIndividuals; i++) {

		//checks if individual is a centroid		
		bool isCentroid = false;
		for (int j = 0; j < k; j++)
			if (centroids[j] == i)
				isCentroid = true;
		if (isCentroid)
			continue;

		//compare individual to centroids
		//if less than minSimilarityMeasure then individual is moved to centroid
		double mostRelatedSimilarityMeasure = 10000000;
		int mostRelatedCentroid;
		for (int j = 0; j < k; j++) {
			
			//if lower similarityMeasure is found then keep track of that centroid	
			if (findIndividualSimilarityMeasure(d, d->values[i], d->values[ centroids[j] ]) < mostRelatedSimilarityMeasure) {
				
				mostRelatedSimilarityMeasure = findIndividualSimilarityMeasure(d, d->values[i], d->values[ centroids[j] ]);
				mostRelatedCentroid = centroids[j];					
		
			}
		}
		//compare lowestSimilarityMeasure achieved through algorithm to 
		//given minimumSimilarityMeasure
		//if less than, the individual will be moved to that centroid
		if (mostRelatedSimilarityMeasure <= minSimilarityMeasure) {
	
			//cout << "Individual " << i << " being moved to centroid " << mostRelatedCentroid << endl;	
			moveIndividualToCentroid(d, i, mostRelatedCentroid);
		}
	}
}

void clusterGenes(Iteration * iteration) {

	int i;

	data * d = iteration->getData();
	int k = iteration->getKForGenes();
	int * centroids = iteration->getGeneCentroids();
	double minSimilarityMeasure = iteration->getGeneSimilarityMeasure();	

}

