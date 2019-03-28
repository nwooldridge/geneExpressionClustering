#include "data.h"

#ifndef ITERATION_H
#define ITERATION_H

class Iteration {

	private:
		data * d;
		int k;
		long * centroids;
		double individualSimilarityMeasure;
		double geneSimilarityMeasure; 

	public:
		Iteration(data *, long, long *);
		data * getData();
		long getK();
		long * getCentroids();
		double getIndividualSimilarityMeasure();
		double getGeneSimilarityMeasure();
};

#endif
