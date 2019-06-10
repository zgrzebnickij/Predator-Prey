#include "Environment.h"
#include "Logger.h"
#include "RandomDevice.h"
#include "ModelGUI.h"
#include "Utilities.h"
#include <map>
#include <algorithm>
#include <iostream>
#include <numeric>


Environment::Environment(int latteSize_, QuantityMap qMap_, const int predMaxHealth_, const int preyHelthToMate_, const int numberOfIterations_, bool blindAgents_):
	latticeSize(latteSize_),
	blindAgents(blindAgents_),
	qMap(qMap_),
	lattice(new Lattice(latticeSize, qMap)),
	predatorMaxHealth(predMaxHealth_),
	preyHelthToMate(preyHelthToMate_),
	numberOfIterations(numberOfIterations_),
	gui(new ModelGUI(lattice->getLattice(), std::bind(&ILattice::checkAgentType, lattice, std::placeholders::_1),
		800, 1200, 800))
{
	//TODO: Move to header
	/*ModelGUI gui(lattice->getLattice(), std::bind(&ILattice::checkAgentType, lattice, std::placeholders::_1),
		800, 1200, 800);
	*/
	system("PAUSE");

	//TODO: Change true for some condition - we don't want infinite loop
	/*for(int i = 0;i<numberOfIterations;i++)
	{
		nextStep();
		finishTurn();
	}*/
}

void Environment::makeIterations() {
	for (int i = 0; i < numberOfIterations; i++)
	{
		nextStep();
		finishTurn();
	}
}

void Environment::nextStep() {
	for (int i = 0; i < latticeSize*latticeSize; i++) {
		Position position(RandomDevice::getInstance().getRandomPosition(latticeSize));

		if (lattice->getAgentID(position)) {
			//TODO: pr�bawa�em to zrobi� ale nie wiem jak. Co� pl�cze ztypami danych
			if (blindAgents) {
				blindAgentTurn(position);
			}
			else {
				sightedAgentTurn(position);
			}
		}
	}
}

void Environment::finishTurn()
{
	for (int i = 0; i < latticeSize; i++) {
		for (int j = 0; j < latticeSize; j++) {
			//TODO: Bedzie zabawa z ownership of pointer
			if (lattice->getAgentID(Position(i, j))) {
				Agent* currentAgent = lattice->getAgentInstance(Position(i, j));
				currentAgent->updateHealth();
				std::cout << lattice->getAgentInstance(Position(i, j))->getHealth() << std::endl;
				if (currentAgent->getAgentType() == Enums::AgentType::Predator && currentAgent->getHealth() <= 0) {
					std::cout << "EKSTERMINACJA WILKA NA (" << i << ", " << j << ")!!!" << std::endl;
					lattice->killAgent(Position(i, j));
				}
			}
		}
	}
}

Environment::Position Environment::generateMovePosition(Position position)
{
	int newRow(-1), newCol(-1);

	do {
		newRow = Utils::BoundaryCondition(position.first + (RandomDevice::getInstance().getRandomInteger(3) - 1), latticeSize);
		newCol = Utils::BoundaryCondition(position.second + (RandomDevice::getInstance().getRandomInteger(3) - 1), latticeSize);
	} while (newCol == position.first && newRow == position.second);

	return Position(newRow, newCol);
}

void Environment::blindAgentTurn(Position agentPosition) {
	Position newPosition = generateMovePosition(agentPosition);
	
	if(lattice->moveAgent(agentPosition, newPosition)) {
		//TODO: Not checked yet. Struggling with killAgent
		checkNeighbours(newPosition);
	}
	else {
		//not sure uf it is necessery
		//checkNeighbours(agentPosition);
	}
}

void Environment::sightedAgentTurn(Position agentPosition) {

}

void Environment::checkNeighbours(Position agentPosition) {
	int agentID = lattice->getAgentID(agentPosition);
	Enums::AgentType currentAgentType = lattice->checkAgentType(agentID);
	double angle = 180;
	double range = 1.01*sqrt(2);
	std::pair<int, int> heading = std::pair(1, 1);
	std::vector<std::pair<int, int>> neighbours = neighboursFromRange(angle, range, heading);
	//TODO: need to shufle neighbours
	for (std::vector<std::pair<int, int>>::iterator it = neighbours.begin(); it != neighbours.end(); ++it) {
		const int newRow = Utils::BoundaryCondition(agentPosition.first + it->first, latticeSize);
		const int newCol = Utils::BoundaryCondition(agentPosition.second + it->second, latticeSize);
		std::pair<int, int> neighbourPosition(newRow, newCol);
		int neighbourAgentID = lattice->getAgentID(neighbourPosition);
		Enums::AgentType neighbourAgentType = lattice->checkAgentType(neighbourAgentID);
		if (currentAgentType == Enums::AgentType::Prey && neighbourAgentType == Enums::AgentType::Prey) {
			// << "Preys are mating";
			//std::cout << "prey mating" << std::endl;
			if (lattice->getAgentInstance(agentPosition)->getHealth() > preyHelthToMate) {
				mating(agentPosition);
				lattice->getAgentInstance(agentPosition)->setHealth(0);
			}
			//create new prey
			break;
		}
		else if (currentAgentType == Enums::AgentType::Predator && neighbourAgentType == Enums::AgentType::Predator) {
			// "Predators are mating";
			//w�asciwie to w naszych za�o�eniach predator rozmana�a si� po zjedzeniu ofiary
			mating(agentPosition);
			//std::cout << "pred mating" << std::endl;
			//creata new Predators
			break;
		}
		else if (currentAgentType > neighbourAgentType && static_cast<int>(neighbourAgentType)!=3) {
			//eat something
			//std::cout << "eating" << std::endl;
			if (lattice->getAgentInstance(agentPosition)->getHealth() < predatorMaxHealth) {
				int preyHealth = lattice->getAgentInstance(neighbourPosition)->getHealth();
				lattice->getAgentInstance(agentPosition)->changeHealth(preyHealth);
			}
			lattice->killAgent(neighbourPosition);
			//tu powinno rozman�anie
			
			// "May eat"; 
			break;
		}
		else {
			//std::cout << static_cast<int>(currentAgentType) << ";" << static_cast<int>(neighbourAgentType) << std::endl;
			//std::cout << "hmm" << std::endl;
		}
	}
}

void Environment::mating(Position agentPosition) {
	int agentID = lattice->getAgentID(agentPosition);
	Enums::AgentType currentAgentType = lattice->checkAgentType(agentID);
	double angle = 180;
	double range = 1.01*sqrt(2);
	std::pair<int, int> heading = std::pair(1, 1);
	std::vector<std::pair<int, int>> neighbours = neighboursFromRange(angle, range, heading);
	for (std::vector<std::pair<int, int>>::iterator it = neighbours.begin(); it != neighbours.end(); ++it) {
		const int newRow = Utils::BoundaryCondition(agentPosition.first + it->first, latticeSize);
		const int newCol = Utils::BoundaryCondition(agentPosition.second + it->second, latticeSize);
		std::pair<int, int> newbornPosition(newRow, newCol);
		int tiletype = lattice->getAgentID(newbornPosition);
		if (tiletype == static_cast<int>(Enums::AgentType::Field) || tiletype == static_cast<int>(Enums::AgentType::Unknown)) {
			lattice->spawnAgent(newbornPosition, Utils::getFreeID(), currentAgentType);
			if (currentAgentType == Enums::AgentType::Predator) {
				int parentHealth = lattice->getAgentInstance(agentPosition)->getHealth();
				lattice->getAgentInstance(newbornPosition)->setHealth(parentHealth);
			}
			break;
		}
	}
}

Environment::PositionsVec Environment::neighboursFromRange(const double visionAngle,
	const double visionRange, const Heading heading) {
	std::stringstream message;
	message << "Checking neighours for (angle, range, heading):("<< visionAngle << ", "<<visionRange << ", (" 
		<< heading.first << ", " << heading.second<< "). ";
	
	PositionsVec neighbours;
	int range = static_cast<int>(std::floor(visionRange));
	double dot, det, angle, distance;

	for (int i = -range; i <= range; i++) {
		for (int j = -range; j <= range; j++) {
			if (j != 0 || i != 0) {
				//make it simpler
				dot = static_cast <double> (heading.first * heading.second + j * i); // dot product between[x1, y1] and [x2, y2]
				det = static_cast <double> (heading.first * j - heading.second * i); //x1 * y2 - y1 * x2      # determinant
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