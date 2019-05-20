#pragma once
class Agent
{
private:
	int health;
	float visionDistance;
	float *visionAngles;
	int *heading;
public:
	Agent();
	~Agent();
	void setHealth(int health);
	int getHealth();
	virtual void updateHealth() = 0;
	void addHealth(int HealthToAdd);
};

