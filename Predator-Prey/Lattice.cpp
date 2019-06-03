#include "Lattice.h"
#include <algorithm>
#include <execution>
#include "Logger.h"
#include "Utilities.h"
#include <boost/format.hpp>
#include "RandomDevice.h"


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
	auto agent = factory.createAgent(agentType);
	changeAgentOnLattice(position, agent->getID());
	agentsVec.push_back(std::move(agent));
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
	//TODO: Usunac z wektora i zwolniæ ID
}

Enums::AgentType Lattice::checkAgentType(int ID)
{
	auto it = std::find_if(std::execution::par, agentsVec.begin(), agentsVec.end(), 
	[=](std::unique_ptr<Agent>& agent)	{ return agent->getID() == ID; });

	if(it != agentsVec.end()) {
		return it->get()->getAgentType();
	}
	return Enums::AgentType::Unknown;
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
	std::for_each(agentsQuantity.begin(), agentsQuantity.end(), [this, &agentVecIter](const QuantityMap::value_type& p)
	{
		std::generate_n(agentVecIter, p.second, [this, &p]()
		{
			auto agent = factory.createAgent(p.first);
			changeAgentOnLattice(generatePosition(), agent->getID());
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
	message << "There is agent with ID: " << agentID << " at position (" << row << ", " <<
		col << ") now.";
	Logger::getInstance().Log("lattice", message.str());
}

Lattice::Position Lattice::generatePosition()
{
	Position position;
	do
	{
		position = RandomDevice::getInstance().getRandomPosition(latticeSize);
	} while (latticeMap[position.first][position.second] != 0);

	return position;
}
