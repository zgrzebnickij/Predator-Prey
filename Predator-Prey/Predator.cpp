#include "Predator.h"



Predator::Predator(const int ID, const int initialHealth) : Agent(Enums::AgentType::Predator){
	setId(ID);
	setHealth(initialHealth); 
}

Predator::~Predator()
{
}

void Predator::updateHealth() {
	//TODO: Avoid magic numbers. Extract to constants
	const int healthToAdd = -1;
	changeHealth(healthToAdd);
}
