#pragma once
#include "Interfaces/IAgent.h"
#include <vector>
#include <string>
#include <functional>
#include "Lattice.h"


class Environment
{
public:
	Environment(int lattSize, bool blindAgents_ = true);
	void nextStep();
	void blindAgentTurn(std::pair<int, int> agentPosition);
	void sightedAgentTurn(std::pair<int, int> agentPosition);
	void checkNeighbours(std::pair<int, int> agentPosition);
	void mating(std::pair<int, int> agentPosition);
	std::vector<std::pair<int, int>> neighboursFromRange(const double visionAngle,
		const double visionRange, const std::pair<int, int> heading);
private:
	int latticeSize;
	bool blindAgents;
	std::shared_ptr<void(std::pair<int, int>)> agentTurn;
	std::shared_ptr<ILattice> lattice;
};

