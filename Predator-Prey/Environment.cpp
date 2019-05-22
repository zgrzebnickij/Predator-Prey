#include <iostream>
#include "Environment.h"
#include "Prey.h"
#include "Predator.h"
#include "boost/multi_array.hpp"
#include <cassert>
#include "boost\shared_ptr.hpp"
#include "BoundaryCondition.h"
//TODO change that for RandomDevice class
#include <ctime>   
#include <cstdlib>



Environment::Environment(int latteSize_, bool blindAgents_):
	latticeSize(latteSize_),
	blindAgents(blindAgents_)
{
	std::map < std::string, int > foodChain;
	//TODO change that
	srand(time(0));
	int values = 1;
	for (int i = 0; i<latticeSize; i++)
	{
		std::vector <int> rowVec;
		for (int j = 0; j<latticeSize; j++)
		{
			if ((i + j) % 4 == 0) {
				rowVec.push_back(values);
				values++;
			}
			else {
				rowVec.push_back(0);
			}
		}
		lattice.push_back(rowVec);
	}
	// Assign values to the elements
	
	for (int i = 0; i < values; i++) {
		if (i % 2) {
			agents.insert(i + 1, new Predator(i + 1, 30));
		}
		else {
			agents.insert(i + 1, new Prey(i + 1));
		}
	}

}

void Environment::printAgents() {
	for (int i = 0; i < agents.size(); i++) {
		std::cout << "Index: " << i << " " << agents[i]->getTypeOfAgent() << " " << agents[i]->getHealth() << std::endl;
	}
}


Environment::~Environment()
{
}


void Environment::nextStep() {
	//TODO 
	for (int i=0; i < 10; i++) {
		const int row = (rand() % latticeSize);
		const int col = (rand() % latticeSize);
		//make a move with object
		std::cout << row << " " << col << std::endl;
		if (lattice[row][col]) {
			agentTurn(row, col);
		}
	}
}

void Environment::agentTurn(const int row, const int col) {
	//TODO: Write a logger for this part and
	const std::string agentType = agents[lattice[row][col]]->getTypeOfAgent();
	if (blindAgents) {
		//ToDO make it a class variable...
		auto cond = BoundaryCondition(latticeSize);

		int newRow = cond.torus(row+(rand() % 3)-1);
		int newCol = cond.torus(col+(rand() % 3)-1);
		//what if it has no place to go?
		while (newCol == col && newRow == row) {
			newRow = cond.torus(row + (rand() % 3) - 1);
			newCol = cond.torus(col + (rand() % 3) - 1);
		}
		std::cout << newRow << " " << newCol << std::endl;
		int placeToMove = lattice[newRow][newCol];
		std::cout << placeToMove << std::endl;
		if (!placeToMove) {
			std::cout << "we are moving" << std::endl;
			lattice[newRow][newCol] = lattice[row][col];
			lattice[row][col] = 0;
			showLattice();
			checkNeighbours(newRow, newCol);
		}
	}
}

void Environment::checkNeighbours(const int row, const int col) {
	std::shared_ptr<Agent> agent = agents[lattice[row][col]];
	std::vector<std::pair<int, int>> neighbours = neighboursFromRange(1);
	std::random_shuffle(neighbours.begin(), neighbours.end());
	for (std::vector<std::pair<int, int>>::iterator it = neighbours.begin(); it != neighbours.end(); ++it)
		std::cout << '(' << it->first << ' ' << it->second << ')';
	std::cout << std::endl;
}

std::vector<std::pair<int, int>> Environment::neighboursFromRange(int range) {
	std::vector<std::pair<int, int>> neighbours;
	for(int i = -range; i <= range; i++) {
		for(int j = -range; j <= range; j++) {
			if (j != 0 || i != 0) {
				neighbours.push_back({ i,j });
			}
		}
	}
	return neighbours;
}




void Environment::showLattice() {
	for (int i = 0; i < latticeSize; ++i) {
		for (int j = 0; j < latticeSize; ++j) {
			std::cout << lattice[i][j] << " ";
		}
		std::cout << std::endl;
	}
}