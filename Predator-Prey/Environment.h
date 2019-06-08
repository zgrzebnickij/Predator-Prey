#pragma once
#include "Interfaces/IAgent.h"
#include "Lattice.h"
#include <vector>
#include <string>
#include <functional>


class Environment
{
public:
	/* 
	*  For Kuba's reviev: Zrobi�em dwa aliasy prowadz�ce do identycznych typ�w, �eby mo�na by�o:
	*   a) �atwiej i czytelniej u�ywa�
	*   b) rozr�ni� do czego odnosi si� dany typ
	*  Usu� ten komentarz jak go przecztasz
	*/
	using Position = std::pair<int, int>;
	using Heading = std::pair<int, int>;
	using PositionsVec = std::vector<Position>;
	using FoodChain = std::map<std::string, int>;
	using AgentType = Enums::AgentType;
	using QuantityMap = std::map<AgentType, int>;

	Environment(int lattSize_, QuantityMap qMap_, bool blindAgents_ = true);

	void nextStep();

	void checkNeighbours(Position agentPosition);
	PositionsVec neighboursFromRange(double visionAngle, double visionRange, Heading heading);

	void blindAgentTurn(Position agentPosition);
	void sightedAgentTurn(Position agentPosition);
	void mating(Position agentPosition);
	void finishTurn();
	Position generateMovePosition(Position position);
private:
	int latticeSize;
	bool blindAgents;
	QuantityMap qMap;
	std::shared_ptr<void(std::pair<int, int>)> agentTurn;
	std::shared_ptr<ILattice> lattice;
};

