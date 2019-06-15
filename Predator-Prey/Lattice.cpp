#include "Lattice.h"
#include <algorithm>
#include <execution>
#include "Logger.h"
#include "Utilities.h"
#include <boost/format.hpp>
#include "RandomDevice.h"
#include <experimental/vector>
#include <mutex>


Lattice::Lattice(int latticeSize_, QuantityMap& agentsQuantity_) :
	latticeSize(latticeSize_),
	latticeMap(latticeSize_),
	agentsQuantity(agentsQuantity_)
{
	generateLattice();
	spawnAgents();
}

Lattice::Matrix* Lattice::getLattice()
{
	return &latticeMap;
}

int Lattice::getAgentID(Position position)
{
	const auto row = Utils::BoundaryCondition(position.first, latticeSize);
	const auto col = Utils::BoundaryCondition(position.second, latticeSize);
	return latticeMap[row][col];
}

Agent* Lattice::getAgentInstance(Position position) {
	int ID = getAgentID(position);

	std::lock_guard<std::mutex> latticeLock(latticeGuard);
	auto it = std::find_if(std::execution::par, agentsVec.begin(), agentsVec.end(),
		[=](std::unique_ptr<Agent>& agent) { return agent->getID() == ID; });
	if (it != agentsVec.end()) {
		return it->get();
	}
	return nullptr;
}

void Lattice::spawnAgent(Position position, int agentID, Enums::AgentType agentType)
{
	auto agent = factory.createAgent(agentType);
	changeAgentOnLattice(position, agent->getID());
	std::lock_guard<std::mutex> latticeLock(latticeGuard);
	agentsVec.push_back(std::move(agent));
}

bool Lattice::moveAgent(Position origin, Position destination)
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

	if (getAgentID(origin) == static_cast<int>(Enums::AgentType::Field)) {
		Logger::getInstance().Log("lattice", "\tCannot move - there is no moving creature on origin position");
		return false;
	}
		
	if (getAgentID(destination) != static_cast<int>(Enums::AgentType::Field)) {
		Logger::getInstance().Log("lattice", "\tCannot move - destination position is occupied");
		return false;
	}

	latticeMap[destRow][destCol] = static_cast<int>(getAgentID(origin));
	latticeMap[originRow][originCol] = static_cast<int>(Enums::AgentType::Field);
	Logger::getInstance().Log("lattice", "\tMoved succesfully");

	return true;
}

void Lattice::killAgent(Position position)
{
	int positionID = getAgentID(position);
	std::lock_guard<std::mutex> latticeLock(latticeGuard);
	std::experimental::erase_if(agentsVec, [this, &positionID](auto const& agent) { 
		auto agentID = agent->getID();
		if (positionID == agentID) {
			Utils::addIDToStack(agentID);
			return true;
		}
		return false;
	});
	changeAgentOnLattice(position, static_cast<int>(Enums::AgentType::Field));
}

Enums::AgentType Lattice::checkAgentType(int ID)
{
	std::lock_guard<std::mutex> latticeLock(latticeGuard);
	auto it = std::find_if(std::execution::par, agentsVec.begin(), agentsVec.end(), 
	[=](std::unique_ptr<Agent>& agent)	{ return agent->getID() == ID; });

	if(it != agentsVec.end()) {
		return it->get()->getAgentType();
	}
	return Enums::AgentType::Unknown;
}

Lattice::QuantityMap Lattice::getAgentStatistics()
{
	QuantityMap statisticMap;
	for (auto i = 1; i < static_cast<int>(Enums::AgentType::Unknown); i++) {
		statisticMap.insert(std::make_pair<Enums::AgentType, int>(static_cast<Enums::AgentType>(i), 0));
	}

	std::lock_guard<std::mutex> latticeLock(latticeGuard);
	std::for_each(agentsVec.begin(), agentsVec.end(), [&statisticMap](auto const& agent) {
		statisticMap[agent->getAgentType()] = statisticMap[agent->getAgentType()] + 1;
	});

	return statisticMap;
}

void Lattice::resetLattice()
{
	for (int row = 0; row < latticeMap.size(); row++) {
		for (int col = 0; col < latticeMap.size(); col++) {
			if (getAgentID(Position(row, col))) {
				killAgent(Position(row, col));
			}
		}
	}

	spawnAgents();
}

void Lattice::updateQuants(QuantityMap qMap_)
{
	agentsQuantity = qMap_;
}

void Lattice::generateLattice()
{
	std::lock_guard<std::mutex> latticeLock(latticeGuard);
	std::generate(std::execution::par, latticeMap.begin(), latticeMap.end(), [this]()
	{
		std::vector<int> rowVec(latticeSize);
		std::generate(std::execution::par, rowVec.begin(), rowVec.end(), []() { return static_cast<int>(Enums::AgentType::Field); });
		return rowVec;
	});
}

void Lattice::spawnAgents()
{
	std::lock_guard<std::mutex> latticeLock(latticeGuard);

	agentsVec.resize(std::accumulate(agentsQuantity.begin(), agentsQuantity.end(), 0,
		[](int value, const QuantityMap::value_type& p) { return value + p.second; }));

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
	do {
		position = RandomDevice::getInstance().getRandomPosition(latticeSize);
	} while (latticeMap[position.first][position.second] != 0);

	return position;
}
