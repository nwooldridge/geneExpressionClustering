#include <iostream>
#include <cstdlib>

#include "Iteration.h"
#include "data.h"

using namespace std;

void findFitness(Iteration ** population, int populationSize) {

	int i;

	int minDataMoved;
	int maxDataMoved;

	for (i = 0; i < populationSize; i++) {
		int dataMoved = population[i]->getAmountOfIndividualsAndGenesClustered();
		if (i == 0) {
			minDataMoved = dataMoved;
			maxDataMoved = dataMoved;
		}
		else if (dataMoved > maxDataMoved)
			maxDataMoved = dataMoved;
		else if (dataMoved < minDataMoved)
			minDataMoved = dataMoved;	
	}

	for (i = 0; i < populationSize; i++) {

		int dataMoved = population[i]->getAmountOfIndividualsAndGenesClustered();
		
		//scaling data moved value to range of 0-99	
		int numerator = dataMoved - minDataMoved;
		int denominator = maxDataMoved - minDataMoved;
		double quotient;	
		if (denominator != 0)
			quotient = (double) numerator / denominator;
		else
			cout << "Divide by zero error\n";
		
		int fitness = 100 * quotient;

		if ((fitness > 100) || (fitness < 0)) {
			fitness = 0;
			cout << "Error: fitness out of range [0,99]\n";
		}
		population[i]->setFitness(fitness);
	}
}
