#pragma once
#include <vector>
#include "Enums.h"

class ILattice
{
public:
	using Matrix = std::vector<std::vector<int>>;

	virtual ~ILattice() = default;
	virtual Matrix getLattice() const = 0;
	virtual int getAgent(std::pair<int, int> position) = 0;
	virtual void spawnAgent(std::pair<int, int> position, Enums::AgentType agentType) = 0;
	virtual void moveAgent(std::pair<int, int> origin, std::pair<int, int> destination) = 0;
	virtual void killAgent(std::pair<int, int> position) = 0;
};
