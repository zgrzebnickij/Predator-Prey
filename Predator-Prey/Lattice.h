#pragma once
#include "Interfaces/ILattice.h"
#include <vector>
#include "Enums.h"

class Lattice : public ILattice
{
public:
	using Matrix = std::vector<std::vector<int>>;

	Lattice(int latticeSize_);

	Matrix* getLattice() override;
	int getAgent(std::pair<int, int> position) override;
	void spawnAgent(std::pair<int, int> position, Enums::AgentType agentType) override;
	void moveAgent(std::pair<int, int> origin, std::pair<int, int> destination) override;
	void killAgent(std::pair<int, int> position) override;

private:
	void generateLattice();
	void changeAgent(std::pair<int, int> position, Enums::AgentType agent_type);

	Matrix				latticeMap;
	int					latticeSize;
};

