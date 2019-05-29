#pragma once
#include "Interfaces/Agent.h"

class Predator : public Agent
{
public:
	Predator(int ID, int initialHealth);
	~Predator();
	void updateHealth() override;
};

