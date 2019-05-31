#include "Predator.h"

Predator::Predator(const int ID_) : Agent(ID_, Enums::AgentType::Predator)
{
}

Predator::~Predator()
{
}

void Predator::updateHealth() {
	//TODO: Avoid magic numbers. Extract to constants
	const int healthToAdd = -1;
	changeHealth(healthToAdd);
}

