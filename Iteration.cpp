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
int * Iteration::getIndividualCentroids() {			return this->individualCentroids;}
void Iteration::setIndividualCentroids(int * ptr) {		this->individualCentroids = ptr;}
int * Iteration::getGeneCentroids() {				return this->geneCentroids;}
void Iteration::setGeneCentroids(int * ptr) {			this->geneCentroids = ptr;}
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
void Iteration::print() {

	
	cout << "Iteration Information:" << endl << "Dataset address:( " << this->d << ") Dataset values(" << this->d->values << "):" << endl;
	cout << "KForIndividuals: " << this->KForIndividuals << endl;
	cout << "Individual Centroids: (" << this->individualCentroids << "):" << endl;
	for (int i = 0; i < this->KForIndividuals; i++)
		cout << this->individualCentroids[i] << " ";
	cout << endl;
	cout << "KForGenes: " << this->KForGenes << endl;
	cout << "Gene Centroids: (" << this->geneCentroids << "):" << endl;
	for (int i = 0; i < this->KForGenes; i++)
                cout << this->geneCentroids[i] << " ";
        cout << endl;
	cout << "IndividualSimilarityMeasure: " << this->individualSimilarityMeasure << endl;
	cout << "GeneSimilarityMeasure: " << this->geneSimilarityMeasure << endl;
	cout << "Amount of Individuals clustered: " << this->amountOfIndividualsClustered << endl;
	cout << "Amount of Genes clustered: " << amountOfGenesClustered << endl;
	cout << "Fitness: " << this->fitness << endl << endl;

}

