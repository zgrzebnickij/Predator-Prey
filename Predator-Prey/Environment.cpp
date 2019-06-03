#include <iostream>
#include "Environment.h"
#include "Prey.h"
#include "Predator.h"
#include <cassert>
#include "Logger.h"
#include <boost/numeric/ublas/vector.hpp>
#include "Lattice.h"
#include "RandomDevice.h"
#include "ModelGUI.h"
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
	std::map<Enums::AgentType, int> qMap;
	qMap.insert(std::pair<Enums::AgentType, int>(Enums::AgentType::Predator, 5));
	qMap.insert(std::pair<Enums::AgentType, int>(Enums::AgentType::Prey, 5));

	std::shared_ptr<ILattice> lattice1(new Lattice(latticeSize, qMap));
	lattice = lattice1;
	ModelGUI gui(lattice->getLattice(), std::bind(&ILattice::checkAgentType, lattice, std::placeholders::_1),
		800, 1200, 800);
	while (true)
	{
		nextStep();
		system("PAUSE");
	}
}


void Environment::nextStep() {
	for (int i = 0; i < latticeSize*latticeSize; i++) {
		std::pair<int, int> position(RandomDevice::getInstance().getRandomPosition(latticeSize));
		if (lattice->getAgent(position)) {
			if (blindAgents) {
				blindAgentTurn(position);
			}
			else {
				sightedAgentTurn(position);
			}
		}
	}
}

void Environment::blindAgentTurn(std::pair<int, int> agentPosition) {
	//ToDO make it a class variable...
	int newRow = Utils::BoundaryCondition(agentPosition.first + (RandomDevice::getInstance().getRandomInteger(3) - 1), latticeSize);
	int newCol = Utils::BoundaryCondition(agentPosition.second + (RandomDevice::getInstance().getRandomInteger(3) - 1), latticeSize);
	while (newCol == agentPosition.first && newRow == agentPosition.second) {
		newRow = Utils::BoundaryCondition(agentPosition.first + (RandomDevice::getInstance().getRandomInteger(3) - 1), latticeSize);
		newCol = Utils::BoundaryCondition(agentPosition.second + (RandomDevice::getInstance().getRandomInteger(3) - 1), latticeSize);
	}
	std::pair<int, int> newPosition(newRow, newCol);
	lattice->moveAgent(agentPosition, newPosition);
	checkNeighbours(newPosition);
	system("PAUSE");
}

void Environment::sightedAgentTurn(std::pair<int, int> agentPosition) {

}

void Environment::checkNeighbours(std::pair<int, int> agentPosition) {
	int agentID = lattice->getAgent(agentPosition);
	Enums::AgentType currentAgentType = lattice->checkAgentType(agentID);
	double angle = 180;
	double range = 1.01*sqrt(2);
	std::pair<int, int> heading = std::pair(1, 1);
	std::vector<std::pair<int, int>> neighbours = neighboursFromRange(angle, range, heading);
	//TODO: need to shufle neighbours
	for (std::vector<std::pair<int, int>>::iterator it = neighbours.begin(); it != neighbours.end(); ++it) {
		const int newRow = Utils::BoundaryCondition(agentPosition.first + it->first, latticeSize);
		const int newCol = Utils::BoundaryCondition(agentPosition.second + it->second, latticeSize);
		std::cout << newRow << "-" << newCol << std::endl;
		std::pair<int, int> neighbourPosition(newRow, newCol);
		int neighbourAgentID = lattice->getAgent(neighbourPosition);
		Enums::AgentType neighbourAgentType = lattice->checkAgentType(neighbourAgentID);
		if (currentAgentType == Enums::AgentType::Prey && neighbourAgentType == Enums::AgentType::Prey) {
			// << "Preys are mating";
			std::cout << "prey mating" << std::endl;
			mating(agentPosition);
			//create new prey
			break;
		}
		else if (currentAgentType == Enums::AgentType::Predator && neighbourAgentType == Enums::AgentType::Predator) {
			// "Predators are mating";
			std::cout << "pred mating" << std::endl;
			mating(agentPosition);
			//creata new Predators
			break;
		}
		else if (currentAgentType > neighbourAgentType && static_cast<int>(neighbourAgentType)!=3) {
			//eat something
			std::cout << "eating" << std::endl;
			lattice->killAgent(neighbourPosition);
			// "May eat"; 
			break;
		}
		else {
			std::cout << static_cast<int>(currentAgentType) << ";" << static_cast<int>(neighbourAgentType) << std::endl;
			std::cout << "hmm" << std::endl;
		}
	}
}

void Environment::mating(std::pair<int, int> agentPosition) {
	int agentID = lattice->getAgent(agentPosition);
	Enums::AgentType currentAgentType = lattice->checkAgentType(agentID);
	double angle = 180;
	double range = 1.01*sqrt(2);
	std::pair<int, int> heading = std::pair(1, 1);
	std::vector<std::pair<int, int>> neighbours = neighboursFromRange(angle, range, heading);
	for (std::vector<std::pair<int, int>>::iterator it = neighbours.begin(); it != neighbours.end(); ++it) {
		const int newRow = Utils::BoundaryCondition(agentPosition.first + it->first, latticeSize);
		const int newCol = Utils::BoundaryCondition(agentPosition.second + it->second, latticeSize);
		std::pair<int, int> newbornPosition(newRow, newCol);
		int tiletype = lattice->getAgent(newbornPosition);
		if (tiletype == static_cast<int>(Enums::AgentType::Field) || tiletype == static_cast<int>(Enums::AgentType::Unknown)) {
			lattice->spawnAgent(newbornPosition, Utils::getFreeID(), currentAgentType);
			break;
		}
	}
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
				//make it simpler
				dot = static_cast <double> (heading_.first * heading_.second + j * i); // dot product between[x1, y1] and [x2, y2]
				det = static_cast <double> (heading_.first * j - heading_.second * i); //x1 * y2 - y1 * x2      # determinant
				angle = atan2(det, dot);
				distance = sqrt(i*i + j * j);
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