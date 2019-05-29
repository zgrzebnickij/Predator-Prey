#pragma once
#include "Interfaces/Agent.h"

class Predator : public Agent
{
public:
	Predator();
	Predator(int ID, int initialHealth);
	~Predator();
	void updateHealth() override;
};

