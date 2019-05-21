#include <iostream>
#include "Environment.h"
#include "Prey.h"
#include "Predator.h"
#include "boost/multi_array.hpp"
#include <cassert>


Environment::Environment(const int latteSize)
{
	latticeSize = latteSize;
	lattice =  new int*[latticeSize];
	for (int i = 0; i < latticeSize; i++) {
		lattice[i] = new int[latticeSize];
	}
	// Assign values to the elements
	int values = 1;
	for (int i = 0; i < latticeSize; ++i) {
		for (int j = 0; j < latticeSize; ++j) {
			if ((i + j) % 4 == 0) {
				lattice[i][j] = values;
				values++;
			}
			else {
				lattice[i][j] = 0;
			}
		}
	}
	for (int i = 0; i < values; i++) {
		if (i % 2) {
			agents.insert(i + 1, new Predator(i + 1, 30));
		}
		else {
			agents.insert(i + 1, new Prey(i + 1));
		}
	}

}



Environment::~Environment()
{
}


void Environment::nextStep() {

}


void Environment::showLattice() {
	for (int i = 0; i < latticeSize; ++i) {
		for (int j = 0; j < latticeSize; ++j) {
			std::cout << lattice[i][j] << " ";
		}
		std::cout << std::endl;
	}
}