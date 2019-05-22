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

Enums::AgentType Lattice::getAgentTypeOnPosition(std::pair<int, int> position)
{
	return static_cast<Enums::AgentType>(latticeMap[position.first][position.second]);
}

void Lattice::spawnAgent(std::pair<int, int> position, Enums::AgentType agentType)
{
	latticeMap[position.first][position.second] = static_cast<int>(agentType);

	std::stringstream message;
	message << "Spawned " << Utils::AgentTypeToString.at(agentType) << " at position (" << position.first << ", " <<
		position.second << ").";
	Logger::getInstance().Log("lattice", message.str());
}

void Lattice::moveAgent(std::pair<int, int> origin, std::pair<int, int> destination)
{
	std::stringstream message;
	message << "Trying to move agent from (" << origin.first << ", " << origin.second << ") to (" << destination.first
		<< ", " << destination.second << ").";
	Logger::getInstance().Log("lattice", message.str());

	//TODO: Separate if to get clearer logs?
	if (getAgentTypeOnPosition(origin) != Enums::AgentType::Grass && getAgentTypeOnPosition(destination) == Enums::AgentType::Grass) {
		latticeMap[destination.first][destination.second] = static_cast<int>(getAgentTypeOnPosition(origin));
		latticeMap[origin.first][origin.second] = static_cast<int>(Enums::AgentType::Grass);
		Logger::getInstance().Log("lattice", "\tMoved succesfully");
	} else {
		Logger::getInstance().Log("lattice", "\tCannot move - agent can't move or destination is occupied");
	}
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
