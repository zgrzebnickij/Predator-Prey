#pragma once
#include "Agent.h"
#include <vector>

class Environment
{
public:
	Environment(const int lattSize);
	~Environment();
	void showLattice();
	void nextStep();
private:
	int latticeSize;
	//TODO: Change it for class lattice
	std::vector<std::shared_ptr<Agent>> agents;
	int **lattice;
};

