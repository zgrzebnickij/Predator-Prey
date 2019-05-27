#include <iostream>
#include "Interfaces/ILattice.h"
#include "Lattice.h"


int main() {
	std::cout << "Predator-Prey Model, Agent-Based Version" << std::endl;

	std::shared_ptr<ILattice> lattice(new Lattice(5));
	const std::pair<int, int> position(1, 2);
	const std::pair<int, int> position2(1, 3);
	const std::pair<int, int> position3(1, 4);

	auto agent = lattice->getAgentTypeOnPosition(position);
	lattice->spawnAgent(position, Enums::AgentType::Predator);
	lattice->spawnAgent(position3, Enums::AgentType::Prey);
	lattice->moveAgent(position, position2);
	lattice->moveAgent(position2, position3);
	lattice->moveAgent(position, position2);
}
