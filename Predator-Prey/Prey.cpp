#include "Prey.h"
#include <string>


//TODO: Add minimal health needed to reproduce
Prey::Prey(const int ID) : Agent("Prey"){
	const int initialHealth = 0;
	setId(ID);
	setHealth(initialHealth);
}

void Prey::updateHealth() {
	//TODO: Avoid magic numbers. Extract to constants
	const int healthToAdd = 1;
	changeHealth(healthToAdd);
}

