#include <iostream>
#include "Environment.h"
#include "Prey.h"
#include "Predator.h"
#include <cassert>
//TODO change that for RandomDevice class
#include <ctime>   
#include <cstdlib>
#include <map>
#include "Utilities.h"
#include <algorithm>


Environment::Environment(int latteSize_, bool blindAgents_) :
	latticeSize(latteSize_),
	blindAgents(blindAgents_)
{
	std::map<std::string, int> foodChain;
	//TODO change that
	srand(time(0));
	int values = 1;
	for (int i = 0; i < latticeSize; i++)
	{
		std::vector <int> rowVec;
		for (int j = 0; j < latticeSize; j++)
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
		std::cout << "Index: " << i << " " << Utils::AgentTypeToString.at(agents[i]->getAgentType()) << " " << agents[i]->getHealth() << std::endl;
	}
}

void Environment::nextStep() {
	//TODO 
	for (int i = 0; i < latticeSize*latticeSize; i++) {
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
	if (blindAgents) {
		//ToDO make it a class variable...
		int newRow = Utils::BoundaryCondition(row + (rand() % 3) - 1, latticeSize);
		int newCol = Utils::BoundaryCondition(col + (rand() % 3) - 1, latticeSize);
		//what if it has no place to go?
		while (newCol == col && newRow == row) {
			newRow = Utils::BoundaryCondition(row + (rand() % 3) - 1, latticeSize);
			newCol = Utils::BoundaryCondition(col + (rand() % 3) - 1, latticeSize);
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
	auto currentType = agent->getAgentType();
	std::vector<std::pair<int, int>> neighbours = neighboursFromRange(1);
	//TODO: Change becuase random_shuffle was removed from C++17 std::random_shuffle(neighbours.begin(), neighbours.end());
	for (std::vector<std::pair<int, int>>::iterator it = neighbours.begin(); it != neighbours.end(); ++it) {
		const int newRow = Utils::BoundaryCondition(row + it->first, latticeSize);
		const int newCol = Utils::BoundaryCondition(col + it->second, latticeSize);
		const int agentIndex = lattice[newRow][newCol];
		if (agentIndex) {
			auto neighbourType = agents[agentIndex]->getAgentType();
			std::cout << "pozycja " << it->first << it->second << std::endl;
			if (currentType == Enums::AgentType::Prey && neighbourType == Enums::AgentType::Prey) {
				//TODO probability of mating
			}
			else if (currentType == Enums::AgentType::Predator && neighbourType == Enums::AgentType::Predator) {
				agent->changeHealth(agents[agentIndex]->getHealth());
				lattice[newRow][newCol] = 0;

			}
			break;
		}
	}
	std::cout << std::endl;
}

std::vector<std::pair<int, int>> Environment::neighboursFromRange(int range) {
	std::vector<std::pair<int, int>> neighbours;
	for (int i = -range; i <= range; i++) {
		for (int j = -range; j <= range; j++) {
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