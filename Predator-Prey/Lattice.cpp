#include "Lattice.h"
#include <algorithm>
#include <execution>


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
}

void Lattice::moveAgent(std::pair<int, int> origin, std::pair<int, int> destination)
{
	if (getAgentTypeOnPosition(origin) != Enums::AgentType::Grass && getAgentTypeOnPosition(destination) == Enums::AgentType::Grass) {
		latticeMap[destination.first][destination.second] = static_cast<int>(getAgentTypeOnPosition(origin));
		latticeMap[origin.first][origin.second] = static_cast<int>(Enums::AgentType::Grass);
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
