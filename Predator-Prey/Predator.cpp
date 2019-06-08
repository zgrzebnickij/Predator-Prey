#include "Predator.h"

Predator::Predator(const int ID_) : Agent(ID_, Enums::AgentType::Predator)
{
}

void Predator::updateHealth() {
	const int healthToAdd = -1;
	changeHealth(healthToAdd);
}

