#include <cmath>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>

#include "data.h"

using namespace std;

static void moveIndividualToCentroid(data * d, long individualIndex, long centroidIndividualIndex) {
	
	long i, j;
	
	vector<double *> beforeArr;
	vector<double *> afterArr;

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
}

static double findSimilarityMeasure(data * d, double * individual, double * centroidIndividual) {
	
	double similarityMeasure = 0;
	

	for (int i = 0; i < d->numGenes; i++) {
		similarityMeasure += abs(individual[i] - centroidIndividual[i]);

	}
	
	return similarityMeasure;

}

static void generateCSV(data * d, string filename) {
	
	ofstream f;
	f.open(filename.c_str(), fstream::out);

	for (int i = 0; i < d->numGenes; i++) {
                for (int j = 0; j < d->numIndividuals; j++) {
                        f << d->values[j][i] << ",";
                }
                f << "\n";
        }
	f.close();	

}

void clusterIndividuals(data * d, long k, long * centroids, double minSimilarityMeasure) {

	long i;
	generateCSV(d, "preAlgorithm.csv");
	
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
	
			cout << "Individual " << i << " being moved to centroid " << mostRelatedCentroid << endl;	
			moveIndividualToCentroid(d, i, mostRelatedCentroid);
		}
	}
	generateCSV(d, "postAlgorithm.csv");
}

