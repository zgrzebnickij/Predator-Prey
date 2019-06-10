#pragma once
#include <vector>
#include "Enums.h"
#include "IAgent.h"
#include <map>

class ILattice
{
public:
	using Matrix = std::vector<std::vector<int>>;
	using Position = std::pair<int, int>;
	using QuantityMap = std::map<Enums::AgentType, int>;

	virtual ~ILattice() = default;
	virtual Matrix* getLattice() = 0;
	virtual int getAgentID(Position position) = 0;
	virtual Agent* getAgentInstance(Position position) = 0;
	virtual void spawnAgent(Position position, int agentID, Enums::AgentType agentType) = 0;
	virtual bool moveAgent(Position origin, Position destination) = 0;
	virtual void killAgent(Position position) = 0;
	virtual Enums::AgentType checkAgentType(int ID) = 0;
	virtual QuantityMap getAgentStatistics() = 0;
};
