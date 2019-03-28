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
	
	long i;
	for (i = 0; i < this->d->numIndividuals; i++)
		delete[] this->d->values[i];
	delete[] this->d->values;

	delete[] this->centroids; 
	
}
data * Iteration::getData() {					return this->d;}
void Iteration::setData(data * d) {				this->d = d;}
int Iteration::getK() {						return this->k;}
void Iteration::setK(int k) {					this->k = k;}
long * Iteration::getCentroids() {				return this->centroids;}
void Iteration::setCentroids(long * ptr) {			this->centroids = ptr;}
double Iteration::getIndividualSimilarityMeasure() {		return this->individualSimilarityMeasure;}
void Iteration::setIndividualSimilarityMeasure(double x) {	this->individualSimilarityMeasure = x;}
double Iteration::getGeneSimilarityMeasure() {			return this->geneSimilarityMeasure;}
void Iteration::setGeneSimilarityMeasure(double x) {		this->geneSimilarityMeasure = x;}
double Iteration::getFitness() {				return this->fitness;}

void Iteration::setFitness(double x){
	
	this->fitness = x;

}
