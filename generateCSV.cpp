#include <string>
#include <fstream>

#include "data.h"
#include "Iteration.h"

using namespace std;

void generateCSV(Iteration ** population, int populationSize, int populationNumber) {

	int maxFitness = -1;
	int mostFitIteration = 0;
	for (int i = 0; i < populationSize; i++) {
			
		if (population[i]->getFitness() > maxFitness) {
			maxFitness = population[i]->getFitness();
			mostFitIteration = i;
		}

	}

	data * d = population[mostFitIteration]->getData();
		
	string filename = "../results/population" + to_string(populationNumber) + "iteration" + to_string(mostFitIteration) + ".csv";

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
