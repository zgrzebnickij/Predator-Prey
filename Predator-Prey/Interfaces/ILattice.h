#pragma once
#include <vector>
#include "Enums.h"
#include "IAgent.h"

class ILattice
{
public:
	using Matrix = std::vector<std::vector<int>>;

	virtual ~ILattice() = default;
	virtual Matrix* getLattice() = 0;
	virtual int getAgent(std::pair<int, int> position) = 0;
	virtual Agent* getAgentInstance(std::pair<int, int> position) = 0;
	virtual void spawnAgent(std::pair<int, int> position, int agentID, Enums::AgentType agentType) = 0;
	virtual bool moveAgent(std::pair<int, int> origin, std::pair<int, int> destination) = 0;
	virtual void killAgent(std::pair<int, int> position) = 0;
	virtual Enums::AgentType checkAgentType(int ID) = 0;
};
