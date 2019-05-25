#pragma once
#include "Agent.h"
#include <vector>
#include <string>
#include <set>
#include <map>
#include "boost\shared_ptr.hpp"

class Environment
{
public:
	Environment(const int lattSize, bool blindAgents_ = true);
	~Environment();
	void showLattice();
	void nextStep();
	void printAgents();
	void agentTurn(const int row, const int col);
	void checkNeighbours(const int row, const int col);
	std::vector<std::pair<int, int>> neighboursFromRange(int range);
private:
	int latticeSize;
	bool blindAgents;
	//TODO: Change it for class lattice
	std::vector<std::shared_ptr<Agent>> agents;
	using Matrix = std::vector<std::vector<int>>;
	Matrix lattice;
};

