#include <iostream>
#include "Environment.h"
#include "Prey.h"
#include "Predator.h"
#include <cassert>
#include "Logger.h"
#include <boost/numeric/ublas/vector.hpp>
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
			agents.push_back(std::shared_ptr<Agent>(new Prey(i+1)));
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
		std::stringstream message;
		message << "Trying to move agent from (" << row << ", " << col << ") to (" << newRow
			<< ", " << newCol << "). ";
		Logger::getInstance().Log("Environment", message.str());
		int placeToMove = lattice[newRow][newCol];
		std::cout << placeToMove << std::endl;
		if (!placeToMove) {
			message << "Succesful agent move from (" << row << ", " << col << ") to (" << newRow
				<< ", " << newCol << ").";
			Logger::getInstance().Log("Environment", message.str());
			lattice[newRow][newCol] = lattice[row][col];
			lattice[row][col] = 0;
			checkNeighbours(newRow, newCol);
			showLattice();
			printAgents();
		}
	}
}

void Environment::checkNeighbours(const int row, const int col) {
	std::shared_ptr<Agent> agent = agents[lattice[row][col]];
	auto currentType = agent->getAgentType();
	double angle = 180;
	double range = 1.01*sqrt(2);
	std::pair<int, int> heading = std::pair(1, 1);
	std::vector<std::pair<int, int>> neighbours = neighboursFromRange(angle, range, heading);
	//TODO: Change becuase random_shuffle was removed from C++17 std::random_shuffle(neighbours.begin(), neighbours.end());
	for (std::vector<std::pair<int, int>>::iterator it = neighbours.begin(); it != neighbours.end(); ++it) {
		const int newRow = Utils::BoundaryCondition(row + it->first, latticeSize);
		const int newCol = Utils::BoundaryCondition(col + it->second, latticeSize);
		const int agentIndex = lattice[newRow][newCol];
		if (agentIndex) {
			auto neighbourType = agents[agentIndex]->getAgentType();
			std::stringstream message;
			message << "Neighbours: " << Utils::AgentTypeToString.at(currentType) << "(" << row << ", "
				<< col << "), vs " << Utils::AgentTypeToString.at(neighbourType) << "(" << newRow<< ", " << newCol << "). ";
			if (currentType == Enums::AgentType::Prey && neighbourType == Enums::AgentType::Prey) {
				message << "Preys are mating";
				//create new prey
			}
			else if (currentType == Enums::AgentType::Predator && neighbourType == Enums::AgentType::Predator) {
				message << "Predators are mating";
				//creata new Predators
				agent->changeHealth(agents[agentIndex]->getHealth());
				lattice[newRow][newCol] = 0;
			}
			else if (currentType > neighbourType) {
				//eat something
				message << "May eat";
				agent->changeHealth(agents[agentIndex]->getHealth());
				lattice[newRow][newCol] = 0;
			}
			Logger::getInstance().Log("Environment", message.str());
			break;
		}
	}
	std::cout << std::endl;
}

std::vector<std::pair<int, int>> Environment::neighboursFromRange(const double visionAngle,
	const double visionRange, const std::pair<int, int> heading_) {
	std::stringstream message;
	message << "neighours for (angle, range, heading):("<< visionAngle << ", "<<visionRange << ", (" 
		<< heading_.first << ", " << heading_.second<< "). ";
	
	std::vector<std::pair<int, int>> neighbours;
	int range = static_cast <int> (std::floor(visionRange));;
	double dot, det, angle, distance;
	for (int i = -range; i <= range; i++) {
		for (int j = -range; j <= range; j++) {
			if (j != 0 || i != 0) {
				dot = static_cast <double> (heading_.first * heading_.second + j * i); // dot product between[x1, y1] and [x2, y2]
				det = static_cast <double> (heading_.first * j - heading_.second * i); //x1 * y2 - y1 * x2      # determinant
				angle = atan2(det, dot);
				distance = sqrt(i*i + j * j);
				std::cout << angle << "'" << distance;
				if ((visionAngle > abs(angle)) && (distance<visionRange)){
					message << "(" << i << ", " << j << ") ";
					neighbours.push_back({ i,j });
				}
			}
		}
	}
	Logger::getInstance().Log("Neighbours", message.str());
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