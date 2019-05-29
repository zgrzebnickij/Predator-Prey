#pragma once
#include "Interfaces/IAgent.h"

class Predator : public Agent
{
public:
	Predator(int ID, int initialHealth);
	~Predator();
	void updateHealth() override;
};

