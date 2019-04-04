#include <iostream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <ctime>

#include "data.h"
#include "parser.h"
#include "clusterAlgorithm.h"
#include "generateBMP.h"
#include "geneticAlgorithm.h"

using namespace std;

int main(int argc, char ** argv) {
	
	srand(time(NULL));

	string s;
	if (argc != 4) {
		cout << "Needs 3 parameters: ./source file numIndividuals numGenes\n";
		exit(-1);
	}
	
	
	s = argv[1];
	data d = parse(s, atoi(argv[2]), atoi(argv[3]));
	d.numIndividuals = atoi(argv[2]);
	d.numGenes = atoi(argv[3]);
	/*
	for (int i = 0; i < d.numIndividuals; i++)
		for (int j = 0; j < d.numGenes; j++)
			cout << d.values[i][j] << " ";
		cout << endl;
	*/
	geneticAlgorithm(50, &d, 25, 8);

	for (int i = 0; i < d.numIndividuals; i++) {
		delete[] d.values[i];
	}

	delete[] d.values;

	return 0;
}
