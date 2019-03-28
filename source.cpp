#include <iostream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <ctime>

#include "data.h"
#include "parser.h"
#include "clusterAlgorithm.h"
#include "generateBMP.h"

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

	//cout << "numIndividuals: " << d.numIndividuals << " numGenes: " << d.numGenes << endl;

	//Uncomment if you want to see all data outputted to console
	/*	
	for (int i = 0; i < d.numGenes; i++) {

		for (int j = 0; j < d.numIndividuals; j++) {			

			cout << d.values[j][i] << " ";

		}

		cout << endl;
	}
	*/

	long centroids[13];
	for (int i = 0; i < 13; i++)
		centroids[i] = rand() % d.numIndividuals;
		
	generateBMP(&d, "preAlgorithm.bmp");

	clusterIndividuals(&d, 13, centroids, 10);
	
	generateBMP(&d, "postAlgorithm.bmp");
	
	for (int i = 0; i < d.numIndividuals; i++) {

		delete[] d.values[i];

	}
	delete[] d.values;

	return 0;
}
