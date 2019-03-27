#include <cmath>
#include <iostream>
#include <cstdlib>

#include "data.h"

using namespace std;

static void moveIndividualToCentroid(data * d, long individualIndex, long centroidIndividualIndex) {
	
	for (int i = 0; i < d->numIndividuals; i++) {

		for (int j = 0; j < d->numGenes; j++)
			cout << d->values[i][j] << " ";
		cout << endl;

	}

	double * individual = d->values[individualIndex];
	double * tempIndividual;

	if (centroidIndividualIndex == (d->numIndividuals - 1)) {

		cout << "centroid at last element" << endl;

	}
	//moves individual with individualIndex 1 to the right of given centroidIndividualIndex
	else {
		tempIndividual = d->values[centroidIndividualIndex + 1];
		d->values[centroidIndividualIndex + 1] = individual;
		for (int i = centroidIndividualIndex + 2; i < d->numIndividuals; i++) {
		
			double * ind1 = d->values[i];
			d->values[i] = tempIndividual;
			tempIndividual = ind1;

		}
	}
	cout << endl << endl << endl;

	for (int i = 0; i < d->numIndividuals; i++) {

                for (int j = 0; j < d->numGenes; j++)
                        cout << d->values[i][j] << " ";
                cout << endl;

        }
 
}

static double findSimilarityMeasure(data * d, double * individual, double * centroidIndividual) {
	
	double similarityMeasure = 0;
	

	for (int i = 0; i < d->numGenes; i++) {
		similarityMeasure += abs(individual[i] - centroidIndividual[i]);

	}
	
	return similarityMeasure;

}

void clusterIndividuals(data * d, long k, long * centroids, double minSimilarityMeasure) {

	long i;
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
		long mostRelatedCentroid;
		for (long j = 0; j < k; j++) {
			
			//if lower similarityMeasure is found then keep track of that centroid	
			if (findSimilarityMeasure(d, d->values[i], d->values[ centroids[j] ]) < mostRelatedSimilarityMeasure) {
				
				mostRelatedSimilarityMeasure = findSimilarityMeasure(d, d->values[i], d->values[ centroids[j] ]);
				mostRelatedCentroid = centroids[j];					
		
			}
		}
		//compare lowestSimilarityMeasure achieved through algorithm to 
		//given minimumSimilarityMeasure
		//if less than, the individual will be moved to that centroid
		if (mostRelatedSimilarityMeasure <= minSimilarityMeasure) {
			
			moveIndividualToCentroid(d, i, mostRelatedCentroid);
			cout << "Individual " << i << " being moved to centroid " << mostRelatedCentroid << endl;

		}
	}
}

