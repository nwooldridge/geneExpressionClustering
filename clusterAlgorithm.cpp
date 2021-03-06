#include <cmath>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>

#include "data.h"
#include "Iteration.h"

using namespace std;

//TODO:
//I realized a very large issue with my code. When moving an individual/gene to a centroid, I need to check whether any centroids are in between the
//individual and centroid in question. If there are any, I need to either increase or decrease the index of that centroid by 1. 

//Splits the array of individuals into two vectors: values before the centroid and values after the centroid.
//This makes it easy to move individuals to the centroid. Just put it at the end of the before values or at the beginning
//of the after values.
static void moveIndividualToCentroid(data * d, int k, cluster * centroids, int individualIndex, int centroidIndex) {
	
	//Actual individual index of the centroid
	int centroidIndividualIndex = centroids[centroidIndex].index;

	int i, j;
	
	vector<double *> beforeArr;
	vector<double *> afterArr;
	
	//Depending on whether the individual being moved is greater than or less than
	//the centroid that it is being moved to, the individual will either be put 
	//just before the centroid or just after. 
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
		
		//Other centroid indices could change if they are between the individual being moved and the centroid
		//the individual is being moved to
		for (i = 0; i < k; i++) {
			if (i == centroidIndex)
				continue;
			if ((centroids[i].index > centroidIndividualIndex) && (centroids[i].index < individualIndex)) {
				centroids[i].index++;
				centroids[i].lowerBoundIndex++;
				centroids[i].upperBoundIndex++;
			}
		}		
	}
	
	else {
		//same as above if statement, except the individual will be just before the centroid
		centroids[centroidIndex].lowerBoundIndex--;
		afterArr.push_back(d->values[centroidIndividualIndex]);

		for (i = 0; i < centroidIndividualIndex; i++) 
			if (i != individualIndex) 
				beforeArr.push_back(d->values[i]);
	
		for (i = centroidIndividualIndex + 1; i < d->numIndividuals; i++)
			if (i != individualIndex)
				afterArr.push_back(d->values[i]);

		beforeArr.push_back(d->values[individualIndex]);	
		
		//Other centroid indices could change if they are between the individual being moved and the centroid
		//the individual is being moved to
		for (i = 0; i < k; i++) {
			if (i == centroidIndex)
				continue;
			if ((centroids[i].index < centroidIndividualIndex) && (centroids[i].index > individualIndex)) {
				centroids[i].index--;
				centroids[i].lowerBoundIndex--;
				centroids[i].upperBoundIndex--;
			}
		}	
	}

	//Reflect changes from this function in original dataset
	int count = 0;
	for (i = 0; i < beforeArr.size(); i++) {
		d->values[count] = beforeArr[i];
		count++;
	}
	
	for (i = 0; i < afterArr.size(); i++) {
		d->values[count] = afterArr[i];
		count++;
	}
}

static void moveGeneToCentroid(data * d, int k, cluster * centroids, int geneIndex, int centroidIndex) {
	
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
		
		for (i = 0; i < k; i++) {
			if (i == centroidIndex)
				continue;
			if ((centroids[i].index > centroidGeneIndex) && (centroids[i].index < geneIndex)) {
				centroids[i].index++;
				centroids[i].lowerBoundIndex++;
				centroids[i].upperBoundIndex++;
			}
		}
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
		
		for (i = 0; i < k; i++) {
			if (i == centroidIndex)
				continue;
			if ((centroids[i].index < centroidGeneIndex) && (centroids[i].index > geneIndex)) {
				centroids[i].index--;
				centroids[i].lowerBoundIndex--;
				centroids[i].upperBoundIndex--;
			}
		}
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

		vector<double *> movedIndividuals; 		
	
		//compare lowestSimilarityMeasure achieved through algorithm to 
		//given minimumSimilarityMeasure
		//if less than, the individual will be moved to that centroid
		if (mostRelatedSimilarityMeasure <= minSimilarityMeasure) {

			bool movedIndividualFlag = false;
			for (j = 0; j < movedIndividuals.size(); j++) 
			{
				if (d->values[i] == movedIndividuals[j])
				{
					movedIndividualFlag = true;
					break;
				}
			}

			if (!movedIndividualFlag) 
			{
				movedIndividuals.push_back(d->values[i]);
				numOfIndividualsMoved++;
				centroids[mostRelatedCentroid].clusterSize++;
				if ((i - centroids[mostRelatedCentroid].index) == 1)
					centroids[mostRelatedCentroid].upperBoundIndex++;
				else if ((i - centroids[mostRelatedCentroid].index) == -1) 
					centroids[mostRelatedCentroid].lowerBoundIndex--;
				else
                         	       moveIndividualToCentroid(d, k, centroids, i, mostRelatedCentroid);
			}
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

		for (j = 0; j < k; j++) { 
			if (findGeneSimilarityMeasure(d, i, centroids[j].index ) < mostRelatedSimilarityMeasure) {
				mostRelatedSimilarityMeasure = findGeneSimilarityMeasure(d, i, centroids[j].index );
				mostRelatedCentroid = j;

			}
		}

		if (mostRelatedSimilarityMeasure <= minSimilarityMeasure) {
			
			numOfGenesMoved++;
			centroids[mostRelatedCentroid].clusterSize++;
			if ((i - centroids[mostRelatedCentroid].index) == 1)
				centroids[mostRelatedCentroid].upperBoundIndex++;
			else if ((i - centroids[mostRelatedCentroid].index) == -1)
				centroids[mostRelatedCentroid].lowerBoundIndex--;
			else
				moveGeneToCentroid(d, k, centroids, i, mostRelatedCentroid);			
		}
	}
	iteration->setAmountOfGenesClustered(numOfGenesMoved);
}

