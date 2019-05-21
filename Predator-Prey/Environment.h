#pragma once
#include "Agent.h"
#include "boost\multi_array.hpp"
<<<<<<< HEAD
#include <vector>
#include <string>
#include "boost\shared_ptr.hpp"

=======
>>>>>>> [WIP] add environment and Predator class

class Environment
{
public:
<<<<<<< HEAD
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

=======
	Environment(const int lattSize);
	~Environment();
	void showLattice();
	void nextStep();
private:
	int latticeSize;
	//TODO: Change it for class lattice
	std::vector<Agent*> agents;
	int **lattice;
>>>>>>> [WIP] add environment and Predator class
};

