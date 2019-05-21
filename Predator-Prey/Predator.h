#pragma once
#include "Agent.h"
class Predator :
	public Agent
{
public:
	Predator(const int ID, const int initialHealth);
	~Predator();
	void updateHealth() override;
};

