#include "Lattice.h"
#include <algorithm>
#include <execution>
#include "Logger.h"
#include "Utilities.h"
#include <boost/format.hpp>


Lattice::Lattice(int latticeSize_) :
	latticeSize(latticeSize_),
	latticeMap(latticeSize_)
{
	generateLattice();
}

Lattice::Matrix Lattice::getLattice() const
{
	return latticeMap;
}

int Lattice::getAgent(std::pair<int, int> position)
{
	const auto row = Utils::BoundaryCondition(position.first, latticeSize);
	const auto col = Utils::BoundaryCondition(position.second, latticeSize);
	return latticeMap[row][col];
}

void Lattice::spawnAgent(std::pair<int, int> position, Enums::AgentType agentType)
{
	changeAgent(position, agentType);
}

void Lattice::moveAgent(std::pair<int, int> origin, std::pair<int, int> destination)
{
	const auto originRow = Utils::BoundaryCondition(origin.first, latticeSize);
	const auto originCol = Utils::BoundaryCondition(origin.second, latticeSize);
	const auto destRow = Utils::BoundaryCondition(destination.first, latticeSize);
	const auto destCol = Utils::BoundaryCondition(destination.second, latticeSize);

	std::stringstream message;
	message << "Trying to move agent from (" << originRow << ", " << originCol << ") to (" << destRow
		<< ", " << destCol << ").";
	Logger::getInstance().Log("lattice", message.str());

	if (getAgent(origin) == static_cast<int>(Enums::AgentType::Grass)) {
		Logger::getInstance().Log("lattice", "\tCannot move - there is no moving creature on origin position");
		return;
	}
		
	if (getAgent(destination) != static_cast<int>(Enums::AgentType::Grass)) {
		Logger::getInstance().Log("lattice", "\tCannot move - destination position is occupied");
		return;
	}

	latticeMap[destRow][destCol] = static_cast<int>(getAgent(origin));
	latticeMap[originRow][originCol] = static_cast<int>(Enums::AgentType::Grass);
	Logger::getInstance().Log("lattice", "\tMoved succesfully");
}

void Lattice::killAgent(std::pair<int, int> position)
{
	changeAgent(position, Enums::AgentType::Grass);
}

void Lattice::generateLattice()
{
	std::generate(std::execution::par, latticeMap.begin(), latticeMap.end(), [this]()
	{
		std::vector<int> rowVec(latticeSize);
		std::generate(std::execution::par, rowVec.begin(), rowVec.end(), []() { return 0; });
		return rowVec;
	});
}

void Lattice::changeAgent(std::pair<int, int> position, Enums::AgentType agent_type)
{
	const auto row = Utils::BoundaryCondition(position.first, latticeSize);
	const auto col = Utils::BoundaryCondition(position.second, latticeSize);
	latticeMap[row][col] = static_cast<int>(agent_type);

	std::stringstream message;
	message << "There is " << Utils::AgentTypeToString.at(agent_type) << " at position (" << row << ", " <<
		col << ") now.";
	Logger::getInstance().Log("lattice", message.str());
}
