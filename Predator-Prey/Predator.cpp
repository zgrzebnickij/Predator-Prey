#include "Predator.h"


<<<<<<< HEAD
Predator::Predator(const int ID, const int initialHealth) : Agent("Predator"){
=======
Predator::Predator(const int ID, const int initialHealth) {
>>>>>>> [WIP] add environment and Predator class
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
