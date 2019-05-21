#pragma once
#include "Agent.h"
#include "boost\multi_array.hpp"

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
	std::vector<Agent*> agents;
	int **lattice;
};

