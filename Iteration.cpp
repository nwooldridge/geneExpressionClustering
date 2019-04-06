#include <iostream>
#include "data.h"
#include "clusterAlgorithm.h"
#include "Iteration.h"

using namespace std;

Iteration::Iteration(data * d) {
	this->d = d;
}

Iteration::~Iteration() {
	
	int i;
	for (i = 0; i < this->d->numIndividuals; i++)
		delete[] this->d->values[i];
	delete[] this->d->values;

	delete[] this->individualCentroids; 
	delete[] this->geneCentroids;
	
}
data * Iteration::getData() {					return this->d;}
void Iteration::setData(data * d) {				this->d = d;}
int Iteration::getKForIndividuals() {				return this->KForIndividuals;}
void Iteration::setKForIndividuals(int k) {			this->KForIndividuals = k;}
int Iteration::getKForGenes() {					return this->KForGenes;}
void Iteration::setKForGenes(int k) {				this->KForGenes = k;}
cluster * Iteration::getIndividualCentroids() {			return this->individualCentroids;}
void Iteration::setIndividualCentroids(cluster * ptr) {		this->individualCentroids = ptr;}
cluster * Iteration::getGeneCentroids() {			return this->geneCentroids;}
void Iteration::setGeneCentroids(cluster * ptr) {		this->geneCentroids = ptr;}
double Iteration::getIndividualSimilarityMeasure() {		return this->individualSimilarityMeasure;}
void Iteration::setIndividualSimilarityMeasure(double x) {	this->individualSimilarityMeasure = x;}
double Iteration::getGeneSimilarityMeasure() {			return this->geneSimilarityMeasure;}
void Iteration::setGeneSimilarityMeasure(double x) {		this->geneSimilarityMeasure = x;}
int Iteration::getFitness() {					return this->fitness;}
void Iteration::setFitness(int x){				this->fitness = x;}
int Iteration::getAmountOfIndividualsClustered() {		return this->amountOfIndividualsClustered;}
void Iteration::setAmountOfIndividualsClustered(int x) {	this->amountOfIndividualsClustered = x;}
int Iteration::getAmountOfGenesClustered() {			return this->amountOfGenesClustered;}
void Iteration::setAmountOfGenesClustered(int x) {		this->amountOfGenesClustered = x;}
int Iteration::getAmountOfIndividualsAndGenesClustered() {	return (this->amountOfIndividualsClustered + this->amountOfGenesClustered);}
void Iteration::print() {

	
	cout << "Iteration Information:" << endl << "Dataset address:( " << this->d << ") Dataset values(" << this->d->values << "):" << endl;
	cout << "KForIndividuals: " << this->KForIndividuals << endl;
	cout << "Individual Centroids (" << this->individualCentroids << "):" << endl;
	for (int i = 0; i < this->KForIndividuals; i++) {
		cout << "\tIndex: " << this->individualCentroids[i].index << endl;
		cout << "\tCluster size: " << this->individualCentroids[i].clusterSize << endl;
		if (this->individualCentroids[i].clusterSize > 0)
			cout << "\tBounds: (" << this->individualCentroids[i].lowerBoundIndex << " - " << this->individualCentroids[i].upperBoundIndex << ")" << endl;
	cout << endl;
	}
	cout << endl;
	cout << "KForGenes: " << this->KForGenes << endl;
	cout << "Gene Centroids (" << this->geneCentroids << "):" << endl;
	for (int i = 0; i < this->KForGenes; i++) {
                cout << "\tIndex: " << this->geneCentroids[i].index << endl;
		cout << "\tCluster size: " << this->geneCentroids[i].clusterSize << endl;
		if (this->geneCentroids[i].clusterSize > 0)
			cout << "\tBounds: (" << this->geneCentroids[i].lowerBoundIndex << " - " <<  this->geneCentroids[i].upperBoundIndex << ")" << endl;
	cout << endl;
	}
	cout << "IndividualSimilarityMeasure: " << this->individualSimilarityMeasure << endl;
	cout << "GeneSimilarityMeasure: " << this->geneSimilarityMeasure << endl;
	cout << "Amount of Individuals clustered: " << this->amountOfIndividualsClustered << endl;
	cout << "Amount of Genes clustered: " << amountOfGenesClustered << endl;
	cout << "Fitness: " << this->fitness << endl << endl;

}

