#include "Agent.h"



Agent::Agent()
{
	health = 0;
	visionDistance = 0.0f;
	/*we have to decide how to present angles and heading. First one i'll do as a array.
	f.e. [0,90,180,270]-it means that agent see from 0 to 90 and from 180 to 270 */
	visionAngles = new float[2];
	// the best case is to have a 2d vector
	heading = new int[2];
}


Agent::~Agent()
{
}


void Agent::setHealth(int health)
{
}


int Agent::getHealth()
{
	return health;
}


void Agent::addHealth(int HealthToAdd)
{
	health += HealthToAdd;
}
