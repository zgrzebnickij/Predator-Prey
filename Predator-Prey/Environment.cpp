#include <iostream>
#include "Environment.h"
#include "Prey.h"
#include "Predator.h"
#include <cassert>
#include "boost\shared_ptr.hpp"
#include "Utilities.h"
#include "Enums.h"
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
			agents.push_back(std::shared_ptr<Agent>(new Predator(i + 1, 30)));
		}
		else {
			agents.push_back(std::shared_ptr<Agent>(new Prey(i + 1)));
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
	for (int i=0; i < latticeSize*latticeSize; i++) {
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
		int newRow = Util::BoundaryCondition(row+(rand() % 3)-1, latticeSize);
		int newCol = Util::BoundaryCondition(col+(rand() % 3)-1, latticeSize);
		//what if it has no place to go?
		while (newCol == col && newRow == row) {
			newRow = Util::BoundaryCondition(row + (rand() % 3) - 1, latticeSize);
			newCol = Util::BoundaryCondition(col + (rand() % 3) - 1, latticeSize);
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
	std::string currentType = agent->getTypeOfAgent();
	std::cout << currentType << std::endl;
	std::vector<std::pair<int, int>> neighbours = neighboursFromRange(1);
	std::random_shuffle(neighbours.begin(), neighbours.end());
	for (std::vector<std::pair<int, int>>::iterator it = neighbours.begin(); it != neighbours.end(); ++it) {
		const int newRow = Util::BoundaryCondition(row + it->first, latticeSize);
		const int newCol = Util::BoundaryCondition(col + it->second, latticeSize);
		const int agentIndex = lattice[newRow][newCol];
		if (agentIndex) {
			std::string neighbourType = agents[agentIndex]->getTypeOfAgent();
			std::cout << "Agent " << neighbourType;
			std::cout << "pozycja " << it->first << it->second << std::endl;
			if (currentType == "Prey" && neighbourType == "Prey") {
				//TODO probability of mating
				std::cout << "Preys mating " << neighbourType;
			}
			else if (currentType == "Predator" && neighbourType == "Prey") {
				agent->changeHealth(agents[agentIndex]->getHealth());
				lattice[newRow][newCol] = 0;
				std::cout << "Predator eat prey" << neighbourType;
				
			}
			break;
		}
		else {
			std::cout << "puste" << std::endl;
		}
	}
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