#include "Lattice.h"
#include <algorithm>
#include <execution>
#include "Logger.h"
#include "Utilities.h"
#include <boost/format.hpp>


Lattice::Lattice(int latticeSize_, QuantityMap& agentsQuantity_) :
	latticeSize(latticeSize_),
	latticeMap(latticeSize_),
	agentsQuantity(agentsQuantity_),
	agentsVec(std::accumulate(agentsQuantity.begin(), agentsQuantity.end(), 0, 
	                       [](int value, const QuantityMap::value_type& p) { return value + p.second; }))
{
	generateLattice();
	spawnAgents();
}

Lattice::Matrix* Lattice::getLattice()
{
	return &latticeMap;
}

int Lattice::getAgent(Position position)
{
	const auto row = Utils::BoundaryCondition(position.first, latticeSize);
	const auto col = Utils::BoundaryCondition(position.second, latticeSize);
	return latticeMap[row][col];
}

void Lattice::spawnAgent(Position position, int agentID, Enums::AgentType agentType)
{
	//TODO: Change hardcoded ID to getter from agent. Need Kuba's implementation for factory
	agentsVec.push_back(factory.createAgent(agentType));
	changeAgentOnLattice(position, agentID);
}

void Lattice::moveAgent(Position origin, Position destination)
{
	//TODO: CHECK
	const auto originRow = Utils::BoundaryCondition(origin.first, latticeSize);
	const auto originCol = Utils::BoundaryCondition(origin.second, latticeSize);
	const auto destRow = Utils::BoundaryCondition(destination.first, latticeSize);
	const auto destCol = Utils::BoundaryCondition(destination.second, latticeSize);

	std::stringstream message;
	message << "Trying to move agent from (" << originRow << ", " << originCol << ") to (" << destRow
		<< ", " << destCol << ").";
	Logger::getInstance().Log("lattice", message.str());

	if (getAgent(origin) == static_cast<int>(Enums::AgentType::Field)) {
		Logger::getInstance().Log("lattice", "\tCannot move - there is no moving creature on origin position");
		return;
	}
		
	if (getAgent(destination) != static_cast<int>(Enums::AgentType::Field)) {
		Logger::getInstance().Log("lattice", "\tCannot move - destination position is occupied");
		return;
	}

	latticeMap[destRow][destCol] = static_cast<int>(getAgent(origin));
	latticeMap[originRow][originCol] = static_cast<int>(Enums::AgentType::Field);
	Logger::getInstance().Log("lattice", "\tMoved succesfully");
}

void Lattice::killAgent(Position position)
{
	changeAgentOnLattice(position, static_cast<int>(Enums::AgentType::Field));
	//TODO: Usunac z wektora
}

void Lattice::generateLattice()
{
	std::generate(std::execution::par, latticeMap.begin(), latticeMap.end(), [this]()
	{
		std::vector<int> rowVec(latticeSize);
		std::generate(std::execution::par, rowVec.begin(), rowVec.end(), []() { return static_cast<int>(Enums::AgentType::Field); });
		return rowVec;
	});
}

void Lattice::spawnAgents()
{
	auto agentVecIter = agentsVec.begin();
	std::for_each(std::execution::par, agentsQuantity.begin(), agentsQuantity.end(), [this, &agentVecIter](const QuantityMap::value_type& p)
	{
		std::generate_n(std::execution::par, agentVecIter, p.second, [this, &p]()
		{
			auto agent = factory.createAgent(p.first);
			//TODO: Rand position and fix ID
			//changeAgentOnLattice(std::pair<int, int>(1, 1), agent->getId());
			return agent;
		});
		std::advance(agentVecIter, p.second);
	});
}

void Lattice::changeAgentOnLattice(Position position, int agentID)
{
	const auto row = Utils::BoundaryCondition(position.first, latticeSize);
	const auto col = Utils::BoundaryCondition(position.second, latticeSize);
	latticeMap[row][col] = agentID;

	std::stringstream message;
	message << "There is " << Utils::AgentTypeToString.at(static_cast<Enums::AgentType>(agentID)) << " at position (" << row << ", " <<
		col << ") now.";
	Logger::getInstance().Log("lattice", message.str());
}
