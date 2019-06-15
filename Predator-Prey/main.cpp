#include <iostream>
#include "Environment.h"

int main() {
	std::cout << "Predator-Prey Model, Agent-Based Version" << std::endl;
	int preys = 30;
	int predators = 10;
	std::map<Enums::AgentType, int> qMap;
	qMap.insert(std::pair<Enums::AgentType, int>(Enums::AgentType::Predator, predators));
	qMap.insert(std::pair<Enums::AgentType, int>(Enums::AgentType::Prey, preys));
	const int iterations = 100;
	const int predatorMaxHealth = 3;
	const int preyHealthToMate = 1;
	const int sizeOfSystem = 15;
	auto env = Environment(sizeOfSystem, qMap, predatorMaxHealth, preyHealthToMate, iterations, false);
	env.makeIterations();
}
