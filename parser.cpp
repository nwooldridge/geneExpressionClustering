#include <fstream>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <string>
#include "data.h"

using namespace std;

//Uses output of Dr. Climer's readExpression algorithm, and parses the data from the normalized values table
data parse(string input, int numIndividuals, int numGenes) {
	
	fstream f;
	
	if (!input.empty())
		f.open(input.c_str());
	else {
		cout << "No file given\n";
		exit(-1);
	}	
	if (!f)
		cout << "Error opening file: " << input << endl;
	
	vector<string> lineArray;
	string line;

	//flag for when to store lines
	bool x = false;

	//iterates through lines and adds them to string vector if it's data	
	while (getline(f, line)) {
						
		if (line == "Normalized values:") 
			x = true;
		
		else if (line == "Timer stopped.")
			x = false;
		else if (x)
			lineArray.push_back(line);
	}
		
	//Delete last line because its just new line
	if (lineArray[lineArray.size() - 1] == "\n")
		lineArray.pop_back();

	//Initialize data 
	data d;
	d.values = new double*[numIndividuals];
	for (int i = 0; i < numIndividuals; i++)
		d.values[i] = new double[numGenes];

	//set values
	int columnIndex = 0;
	for (int i = 0; i < lineArray.size(); i++) {
		
		int rowIndex = 0;
		string temp = "";
		//iterates through characters to turn strings into doubles
		for (int j = 0; j < lineArray[i].size(); j++) {
			
			//row and column index should match given arguments
			if ((rowIndex >= numIndividuals) || (columnIndex >= numGenes)) {
				cout << "Bounds given doesn't match data\n";
				exit(-1);

			}
			if (isspace(lineArray[i][j])) {
				
				if (lineArray[i][j] == 10) {
					
					d.values[rowIndex][columnIndex] = stod(temp.c_str());
					break;

				}
				else if (lineArray[i][j] == 32) {
				
					d.values[rowIndex][columnIndex] = stod(temp.c_str());
					temp = "";
					rowIndex++;
					continue;

				}				
			}
			else if ((isdigit(lineArray[i][j])) || (lineArray[i][j] == '-') || (lineArray[i][j] == '.') || (lineArray[i][j] == 'e')) {
				temp += lineArray[i][j];
	
			}	
		}
		columnIndex++;
	}

	return d;
}
