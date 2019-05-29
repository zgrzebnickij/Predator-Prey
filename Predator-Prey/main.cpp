#include <iostream>
#include "Interfaces/ILattice.h"
#include "Lattice.h"
#include "ModelGUI.h"
#include "Logger.h"

int main() {
	std::cout << "Predator-Prey Model, Agent-Based Version" << std::endl;

	std::map<Enums::AgentType, int> qMap;
	qMap.insert(std::pair<Enums::AgentType, int>(Enums::AgentType::Predator, 5));
	qMap.insert(std::pair<Enums::AgentType, int>(Enums::AgentType::Prey, 10));


	std::unique_ptr<ILattice> lattice(new Lattice(5, qMap));
	ModelGUI gui(lattice->getLattice(), 800, 1200, 800);
	const std::pair<int, int> position(4, 4);
	const std::pair<int, int> position2(4, 5);
	const std::pair<int, int> position3(4, 6);

	auto agent = lattice->getAgent(position);
	lattice->spawnAgent(position, 1, Enums::AgentType::Predator);
	lattice->spawnAgent(position3, 2, Enums::AgentType::Prey);
	lattice->moveAgent(position, position2);
	lattice->moveAgent(position2, position3);
	lattice->moveAgent(position, position2);
	//lattice->killAgent(position2);
}
