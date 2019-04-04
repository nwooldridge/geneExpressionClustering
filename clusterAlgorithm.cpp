#include <cmath>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>

#include "data.h"
#include "Iteration.h"

using namespace std;

static void moveIndividualToCentroid(data * d, cluster * centroids, int individualIndex, int centroidIndex) {
	
	int centroidIndividualIndex = centroids[centroidIndex].index;

	int i, j;
	
	vector<double *> beforeArr;
	vector<double *> afterArr;
	
	//cout << "before moving: " << d->values[ centroidIndividualIndex ];

	if (individualIndex > centroidIndividualIndex) {
		centroids[centroidIndex].upperBoundIndex++;
		afterArr.push_back(d->values[individualIndex]);	

		for (i = 0; i < centroidIndividualIndex; i++)
                	if (i != individualIndex)
                        	beforeArr.push_back(d->values[i]);
		for (i = centroidIndividualIndex + 1; i < d->numIndividuals; i++)
              		if (i != individualIndex)
                        	afterArr.push_back(d->values[i]);

		beforeArr.push_back(d->values[centroidIndividualIndex]);
		
	}
	else {
		centroids[centroidIndex].lowerBoundIndex--;
		afterArr.push_back(d->values[centroidIndividualIndex]);

		for (i = 0; i < centroidIndividualIndex; i++) 
			if (i != individualIndex) 
				beforeArr.push_back(d->values[i]);
	
		for (i = centroidIndividualIndex + 1; i < d->numIndividuals; i++)
			if (i != individualIndex)
				afterArr.push_back(d->values[i]);

		beforeArr.push_back(d->values[individualIndex]);		
	}
	
	int count = 0;
	for (i = 0; i < beforeArr.size(); i++) {
		d->values[count] = beforeArr[i];
		count++;
	}
	
	for (i = 0; i < afterArr.size(); i++) {
		d->values[count] = afterArr[i];
		count++;
	}
	
	
	//cout << " after moving: " << d->values[ centroidIndividualIndex ] << endl;
}

static void moveGeneToCentroid(data * d, cluster * centroids, int geneIndex, int centroidIndex) {
	
	int centroidGeneIndex = centroids[centroidIndex].index;

	//cout << "Moving gene " << geneIndex << " to gene centroid " << centroidGeneIndex << endl;
	

	int i, j;

	//creates duplicate array except essentially rows and columns are switched, so I can move rows of
	//genes around like I moved the individuals around in the other function.
	double ** swappedArr = new double*[d->numGenes];
	for (i = 0; i < d->numGenes; i++) {
		swappedArr[i] = new double[d->numIndividuals];
		for (j = 0; j < d->numIndividuals; j++) {
			swappedArr[i][j] = d->values[j][i];
		}
	}
	vector <double *> beforeArr, afterArr;
	
	if (geneIndex > centroidGeneIndex) {
		centroids[centroidIndex].upperBoundIndex++;
		afterArr.push_back(swappedArr[geneIndex]);

		for (i = 0; i < centroidGeneIndex; i++)
        	        if (i != geneIndex)
                       		beforeArr.push_back(swappedArr[i]);
		for (i = centroidGeneIndex + 1; i < d->numGenes; i++)
                	if (i != geneIndex)
                        	afterArr.push_back(swappedArr[i]);
		beforeArr.push_back(swappedArr[centroidGeneIndex]);
	}
	else {
		centroids[centroidIndex].lowerBoundIndex--;
		afterArr.push_back(swappedArr[centroidGeneIndex]);
		for (i = 0; i < centroidGeneIndex; i++)
                	if (i != geneIndex)
                        	beforeArr.push_back(swappedArr[i]);		
		for (i = centroidGeneIndex + 1; i < d->numGenes; i++)
                	if (i != geneIndex)
                        	afterArr.push_back(swappedArr[i]);
		beforeArr.push_back(swappedArr[geneIndex]);
	}

	int count = 0;
	for (i = 0; i < beforeArr.size(); i++) {
		swappedArr[count] = beforeArr[i];
		count++;
	}
	for (i = 0; i < afterArr.size(); i++) {
		swappedArr[count] = afterArr[i];
		count++;
	}

	for (i = 0; i < d->numIndividuals; i++)
		for (j = 0; j < d->numGenes; j++)
			d->values[i][j] = swappedArr[j][i];
		
	for (i = 0; i < d->numGenes; i++)
		delete[] swappedArr[i];
	delete[] swappedArr;	

}

static double findIndividualSimilarityMeasure(data * d, double * individual, double * centroidIndividual) {
	
	double similarityMeasure = 0;
	
	for (int i = 0; i < d->numGenes; i++) 
		similarityMeasure += abs(individual[i] - centroidIndividual[i]);

	
	
	return similarityMeasure;

}

static double findGeneSimilarityMeasure(data * d, int geneIndex, int centroidGeneIndex) {

	double similarityMeasure = 0;

	for (int i = 0; i < (d->numIndividuals); i++) 
		similarityMeasure += abs((d->values[i][geneIndex]) - (d->values[i][centroidGeneIndex]));
	
	
		
	return similarityMeasure;
}

void clusterIndividuals(Iteration * iteration) {

	int i, j;

	data * d = iteration->getData();
	int k = iteration->getKForIndividuals();
	cluster * centroids = iteration->getIndividualCentroids();
	double minSimilarityMeasure = iteration->getIndividualSimilarityMeasure();

	int numOfIndividualsMoved = 0;
		
	for (i = 0; i < d->numIndividuals; i++) {

		//checks if individual is a centroid		
		bool isCentroid = false;
		for (j = 0; j < k; j++)
			if (centroids[j].index == i)
				isCentroid = true;
		if (isCentroid)
			continue;

		//compare individual to centroids
		//if less than minSimilarityMeasure then individual is moved to centroid
		double mostRelatedSimilarityMeasure = 10000000;
		int mostRelatedCentroid;
		for (j = 0; j < k; j++) {
			
			//if lower similarityMeasure is found then keep track of that centroid	
			if (findIndividualSimilarityMeasure(d, d->values[i], d->values[ centroids[j].index ]) < mostRelatedSimilarityMeasure) {
				mostRelatedSimilarityMeasure = findIndividualSimilarityMeasure(d, d->values[i], d->values[ centroids[j].index ]);
				mostRelatedCentroid = j;					
		
			}
		}

		//cout << mostRelatedSimilarityMeasure << " ?<= " << minSimilarityMeasure << endl;
		
		//compare lowestSimilarityMeasure achieved through algorithm to 
		//given minimumSimilarityMeasure
		//if less than, the individual will be moved to that centroid
		if (mostRelatedSimilarityMeasure <= minSimilarityMeasure) {
			numOfIndividualsMoved++;
			centroids[mostRelatedCentroid].clusterSize++;
			if ((i - centroids[mostRelatedCentroid].index) == 1)
				centroids[mostRelatedCentroid].upperBoundIndex++;
			else if ((i - centroids[mostRelatedCentroid].index) == -1) 
				centroids[mostRelatedCentroid].lowerBoundIndex--;
			else
                                moveIndividualToCentroid(d, centroids, i, mostRelatedCentroid);
		}
	}

	iteration->setAmountOfIndividualsClustered(numOfIndividualsMoved);
}

void clusterGenes(Iteration * iteration) {

	int i, j;

	data * d = iteration->getData();
	int k = iteration->getKForGenes();
	cluster * centroids = iteration->getGeneCentroids();
	double minSimilarityMeasure = iteration->getGeneSimilarityMeasure();	

	int numOfGenesMoved = 0;
	
	for (i = 0; i < d->numGenes; i++) {

		bool isCentroid = false;
		for (j = 0; j < k; j++)
			if (centroids[j].index == i)
				isCentroid = true;
		if (isCentroid)
			continue;		
		
		double mostRelatedSimilarityMeasure = 1000000;
		int mostRelatedCentroid;

		for (j = 0; j < k; j++) 
			if (findGeneSimilarityMeasure(d, i, centroids[j].index ) < mostRelatedSimilarityMeasure) {
				mostRelatedSimilarityMeasure = findGeneSimilarityMeasure(d, i, centroids[j].index );
				mostRelatedCentroid = j;

			}
		
		if (mostRelatedSimilarityMeasure <= minSimilarityMeasure) {
			numOfGenesMoved++;
			centroids[mostRelatedCentroid].clusterSize++;
			if ((i - centroids[mostRelatedCentroid].index) == 1)
				centroids[mostRelatedCentroid].upperBoundIndex++;
			else if ((i - centroids[mostRelatedCentroid].index) == -1)
				centroids[mostRelatedCentroid].lowerBoundIndex--;
			else
				moveGeneToCentroid(d, centroids, i, mostRelatedCentroid);
			
			
		}

	}
	
	iteration->setAmountOfGenesClustered(numOfGenesMoved);

}

