#pragma once
#include "Interfaces/ILattice.h"
#include <vector>
#include "Enums.h"

class Lattice : public ILattice
{
public:
	using Matrix = std::vector<std::vector<int>>;

	Lattice(int latticeSize_);

	Matrix getLattice() const override;
	Enums::AgentType getAgentTypeOnPosition(std::pair<int, int> position) override;
	void spawnAgent(std::pair<int, int> position, Enums::AgentType agentType) override;
	void moveAgent(std::pair<int, int> origin, std::pair<int, int> destination) override;

private:
	void generateLattice();

	Matrix				latticeMap;
	int					latticeSize;
};

