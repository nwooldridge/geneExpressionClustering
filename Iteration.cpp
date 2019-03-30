#include <iostream>
#include "data.h"
#include "clusterAlgorithm.h"
#include "Iteration.h"

using namespace std;

Iteration::Iteration(data * d) {
	this->d = d;
}
/*
Iteration::Iteration(data * d, int k, long * centroids, double individualSimilarityMeasure, double geneSimilarityMeasure) {

	this->d = d;
	this->k = k;
	this->centroids = centroids;
	this->individualSimilarityMeasure = individualSimilarityMeasure;
	this->geneSimilarityMeasure = geneSimilarityMeasure;

	clusterIndividuals(d, k, centroids, individualSimilarityMeasure);

}
*/
Iteration::~Iteration() {
	
	int i;
	for (i = 0; i < this->d->numIndividuals; i++)
		delete[] this->d->values[i];
	delete[] this->d->values;

	delete[] this->centroids; 
	
}
data * Iteration::getData() {					return this->d;}
void Iteration::setData(data * d) {				this->d = d;}
int Iteration::getK() {						return this->k;}
void Iteration::setK(int k) {					this->k = k;}
int * Iteration::getCentroids() {				return this->centroids;}
void Iteration::setCentroids(int * ptr) {			this->centroids = ptr;}
double Iteration::getIndividualSimilarityMeasure() {		return this->individualSimilarityMeasure;}
void Iteration::setIndividualSimilarityMeasure(double x) {	this->individualSimilarityMeasure = x;}
double Iteration::getGeneSimilarityMeasure() {			return this->geneSimilarityMeasure;}
void Iteration::setGeneSimilarityMeasure(double x) {		this->geneSimilarityMeasure = x;}
int Iteration::getFitness() {					return this->fitness;}
void Iteration::setFitness(int x){				this->fitness = x;}
void Iteration::print() {

	
	cout << "Iteration Information:" << endl << "Dataset address:( " << this->d << ") Dataset values(" << this->d->values << "):" << endl;
	cout << "K: " << this->k << endl;
	cout << "Centroids: (" << this->centroids << "):" << endl;
	for (int i = 0; i < this->k; i++)
		cout << this->centroids[i] << " ";
	cout << endl;
	cout << "IndividualSimilarityMeasure: " << this->individualSimilarityMeasure << endl;
	cout << "GeneSimilarityMeasure: " << this->geneSimilarityMeasure << endl;
	cout << "Fitness: " << this->fitness << endl << endl;

}

