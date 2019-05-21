#pragma once
#include <utility>

class Agent
{
public:
	Agent() : health(0), visionDistance(0), visionAngle(180) {}
	virtual ~Agent() = default;
	void setHealth(const int healthToSet) { health = healthToSet; }
	int getHealth() const { return health; }
	virtual void updateHealth() = 0;
	void changeHealth(const int healthToAdd) { health += healthToAdd; }

private:
	int health;
	double visionDistance;
	int visionAngle;
	std::pair<int, int> heading;
};

