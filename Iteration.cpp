#include "data.h"
#include "clusterAlgorithm.h"
#include "Iteration.h"

using namespace std;

Iteration::Iteration(data * d, long k, long * centroids, double individualSimilarityMeasure, double geneSimilarityMeasure) {

	this->d = d;
	this->k = k;
	this->centroids = centroids;
	this->individualSimilarityMeasure = individualSimilarityMeasure;
	this->geneSimilarityMeasure = geneSimilarityMeasure;


}
data * Iteration::getData() {				return this->d;}
long Iteration::getK() {				return this->k;}
long * Iteration::getCentroids() {			return this->centroids;}
double Iteration::getIndividualSimilarityMeasure() {	return this->individualSimilarityMeasure;}
double Iteration::getGeneSimilarityMeasure() {		return this->geneSimilarityMeasure;}
