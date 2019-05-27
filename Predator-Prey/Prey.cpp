#include "Prey.h"


//TODO: Add minimal health needed to reproduce
Prey::Prey(const int ID) : Agent(Enums::AgentType::Prey){
	setId(ID);
}

void Prey::updateHealth() {
	const int healthToAdd = 1;
	changeHealth(healthToAdd);
}

