#include "Predator.h"


<<<<<<< HEAD
<<<<<<< HEAD
Predator::Predator(const int ID, const int initialHealth) : Agent("Predator"){
=======
Predator::Predator(const int ID, const int initialHealth) {
>>>>>>> [WIP] add environment and Predator class
=======
Predator::Predator(const int ID, const int initialHealth) : Agent("Predator"){
>>>>>>> add Boundary condition, nearest neightbours..
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
