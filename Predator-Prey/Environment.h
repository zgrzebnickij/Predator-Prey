#pragma once
#include "Interfaces/Agent.h"
#include <vector>
#include <string>


class Environment
{
public:
	using Matrix = std::vector<std::vector<int>>;

	Environment(int lattSize, bool blindAgents_ = true);
	void showLattice();
	void nextStep();
	void printAgents();
	void agentTurn(int row, int col);
	void checkNeighbours(int row, int col);
	std::vector<std::pair<int, int>> neighboursFromRange(int range);
private:
	int latticeSize;
	bool blindAgents;
	//TODO: Change it for class lattice
	std::vector<std::shared_ptr<Agent>> agents;
	Matrix lattice;
};

