#include "data.h"

#ifndef ITERATION_H
#define ITERATION_H

struct cluster {
	int index;
	int lowerBoundIndex;
	int upperBoundIndex;
	int clusterSize;
};

class Iteration {

	private:
		data * d;
		int KForIndividuals;
		int KForGenes;
		cluster * individualCentroids;
		cluster * geneCentroids;
		double individualSimilarityMeasure;
		double geneSimilarityMeasure;
		int fitness;
		int amountOfIndividualsClustered;
		int amountOfGenesClustered;

	public:
/*------------------------------------------------------------
 *		Constructor and Destructor		
-------------------------------------------------------------*/
		Iteration(data *);
		~Iteration();

/*------------------------------------------------------------
 *		Getters and Setters
-------------------------------------------------------------*/
		data * getData();
		void setData(data *);
		int getKForIndividuals();
		void setKForIndividuals(int);
		int getKForGenes();
		void setKForGenes(int);

		cluster * getIndividualCentroids();
		void setIndividualCentroids(cluster *);
		cluster * getGeneCentroids();
		void setGeneCentroids(cluster *);

		double getIndividualSimilarityMeasure();
		void setIndividualSimilarityMeasure(double);
		double getGeneSimilarityMeasure();
		void setGeneSimilarityMeasure(double);
		int getFitness();
		void setFitness(int);
		int getAmountOfIndividualsClustered();
		void setAmountOfIndividualsClustered(int);
		int getAmountOfGenesClustered();
		void setAmountOfGenesClustered(int);

		void print();
		
		
};
#endif
