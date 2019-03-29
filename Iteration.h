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
		int fitness; 

	public:
		Iteration(data *);
		//Iteration(data *, int, long *, double, double);
		~Iteration();
		data * getData();
		void setData(data *);
		int getK();
		void setK(int);
		long * getCentroids();
		void setCentroids(long *);
		double getIndividualSimilarityMeasure();
		void setIndividualSimilarityMeasure(double);
		double getGeneSimilarityMeasure();
		void setGeneSimilarityMeasure(double);
		int getFitness();
		void setFitness(int);
		void print();
		
		
};
#endif
