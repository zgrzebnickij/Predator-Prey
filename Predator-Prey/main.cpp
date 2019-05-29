#include <iostream>
#include "Interfaces/ILattice.h"
#include "Lattice.h"
#include "ModelGUI.h"
#include "Logger.h"
#include "Environment.h"

int main() {
	std::cout << "Predator-Prey Model, Agent-Based Version" << std::endl;
	auto env = Environment(6);
	env.showLattice();
	env.printAgents();
	env.nextStep();
	env.nextStep();

	std::cout << "Hello world " << std::endl;
	Logger::getInstance().Log("test", "Przyklad loggera");
	Logger::getInstance().Log("test", "Tak go uzywac");
	Logger::getInstance().Log("inny_log", "Pliki sa tworzone i dopisywane na koncu");

	std::unique_ptr<ILattice> lattice(new Lattice(5));
	ModelGUI gui(lattice->getLattice(), 800, 1200, 800);
	const std::pair<int, int> position(4, 4);
	const std::pair<int, int> position2(4, 5);
	const std::pair<int, int> position3(4, 6);

	auto agent = lattice->getAgent(position);
	lattice->spawnAgent(position, Enums::AgentType::Predator);
	lattice->spawnAgent(position3, Enums::AgentType::Prey);
	lattice->moveAgent(position, position2);
	lattice->moveAgent(position2, position3);
	lattice->moveAgent(position, position2);
	//lattice->killAgent(position2);
}
