#pragma once
#include "Interfaces/IAgent.h"
#include <vector>
#include <string>
#include <functional>
#include "Lattice.h"


class Environment
{
public:
	using Position = std::pair<int, int>;

	Environment(int lattSize, bool blindAgents_ = true);
	void nextStep();
	void blindAgentTurn(Position);
	void sightedAgentTurn(Position);
	void checkNeighbours(Position);
	std::vector<std::pair<int, int>> neighboursFromRange(const double visionAngle,
		const double visionRange, const std::pair<int, int> heading);
private:
	int latticeSize;
	bool blindAgents;
	std::function<void(Position)> agentTurn;
	std::shared_ptr<ILattice> lattice;
};

