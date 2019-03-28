#include <cmath>
#include <string>
#include <iostream>

#include "../bitmap/bitmap_image.hpp"
#include "data.h"

using namespace std;

static double * findQ1AndQ3(data * d) {

        long i, j;

        double * returnResult = new double[3];

        double * values = new double[(d->numIndividuals * d->numGenes)];

        long count = 0;
        for (i = 0; i < d->numIndividuals; i++) {

                for (j = 0; j < d->numGenes; j++) {

                        values[count] = d->values[i][j];
                        count++;
                }
        }

        //bubblesort for simplicity
        bool swap = true;
        while (swap) {

                swap = false;

                for (i = 0; i < (d->numIndividuals * d->numGenes); i++) {

                        if (i == (d->numIndividuals * d->numGenes - 1))
                                continue;
                        if (values[i] > values[i+1]) {

                                double temp = values[i];
                                values[i] = values[i+1];
                                values[i+1] = temp;
                                swap=true;
                        }
                }
        }
        returnResult[0] = values[(((d->numIndividuals * d->numGenes) / 2) / 2)];
        returnResult[1] = values[((d->numIndividuals * d->numGenes) / 2)];
        returnResult[2] = values[(((d->numIndividuals * d->numGenes) / 2)) + ((((d->numIndividuals * d->numGenes) / 2)) / 2)];
        delete[] values;
        return returnResult;
}



void generateBMP(data * d, string filename) {

        bitmap_image image(d->numIndividuals, d->numGenes);
        image.set_all_channels(0,0,0);
	
	double * quartiles = findQ1AndQ3(d);
	double q1 = quartiles[0];
	double median = quartiles[1];
	double q3 = quartiles[2];
	
	delete[] quartiles;
	
        //setting pixel color
        for (int i = 0; i < d->numIndividuals; i++) {

                for (int j = 0; j < d->numGenes; j++) {
                        unsigned char r,g,b;
                        if (d->values[i][j] < median) {
                                r = 255;
                                if (d->values[i][j] <= q1) {
                                        g = 0;
                                        b = 0;
                                }
                                else {
                                        double x =(165/ abs(median-q1)) * (abs(d->values[i][j] - q1));
                                        //cout << x << " " << 165 - x << endl;
                                        g = 165-x;
                                        b = 165-x;

                                }

                        }
                        else {
                                b = 255;
                                if (d->values[i][j] >= q3) {
                                        g = 0;
                                        r = 0;
                                }
                                else {
                                        double x = (165/ abs(q3 - median)) * (abs(q3 - d->values[i][j]));
                                        //cout << x << " " << 165-x << endl;
                                        g = 165-x;
                                        r = 165-x;
                                }
                        }
                        if ((r < 0) || (g < 0) || (b < 0) || (g > 165))
                                cout << "error\n";
                        image.set_pixel(i, j, r,g,b);
                }
        }
        image.save_image(filename);
}
