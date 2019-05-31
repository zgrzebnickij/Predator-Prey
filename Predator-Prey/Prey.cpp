#include "Prey.h"

Prey::Prey(const int ID_) : Agent(ID_, Enums::AgentType::Prey)
{
}


void Prey::updateHealth() {
	const int healthToAdd = 1;
	changeHealth(healthToAdd);
}

