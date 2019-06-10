#pragma once
#include "Interfaces/ILattice.h"
#include <vector>
#include "Enums.h"
#include <map>
#include "Interfaces/IAgent.h"
#include <memory>
#include "AgentFactory.h"
#include <mutex>

class Lattice : public ILattice
{
public:
	using Matrix = std::vector<std::vector<int>>;
	using AgentVec = std::vector<std::unique_ptr<Agent>>;
	using QuantityMap = std::map<Enums::AgentType, int>;
	using Position = std::pair<int, int>;

	Lattice(int latticeSize_, QuantityMap& agentsQuantity_);

	Matrix* getLattice() override;
	int getAgentID(Position position) override;
	Agent* getAgentInstance(Position position) override;
	void spawnAgent(Position position, int agentID, Enums::AgentType agentType) override;
	bool moveAgent(Position origin, Position destination) override;
	void killAgent(Position position) override;
	Enums::AgentType checkAgentType(int ID) override;

private:
	void generateLattice();
	void spawnAgents();
	void changeAgentOnLattice(Position position, int agentID);
	Position generatePosition();

	int				latticeSize;
	Matrix			latticeMap;
	QuantityMap		agentsQuantity;
	AgentVec		agentsVec;

	AgentFactory	factory;

	std::mutex      latticeGuard;
};

