#pragma once
#include "Interfaces/IAgent.h"

class Predator : public Agent
{
public:
	Predator(int ID_);
	~Predator();
	void updateHealth() override;
};

