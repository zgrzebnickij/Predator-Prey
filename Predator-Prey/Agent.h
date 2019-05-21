#pragma once
class Agent
{
public:
	Agent();
	virtual ~Agent();
	inline void setHealth(int healthToSet) { health = healthToSet; }
	inline int getHealth() { return health; }
	virtual void updateHealth() = 0;
	inline void addHealth(int healthToAdd) { health += healthToAdd; }
private:
	int health;
	float visionDistance;
	int visionAngle;
	int *heading;
};

