#pragma once
#include <utility>
#include "Enums.h"

class Agent
{
public:
	Agent(Enums::AgentType agentType_) : agentType(agentType_), health(0), visionDistance(0), visionAngle(180) {}
	virtual ~Agent() = default;

	Enums::AgentType getAgentType() const { return agentType; }
	void setHealth(const int healthToSet) { health = healthToSet; }
	int getHealth() const { return health; }
	void setId(const int ID) { id = ID; }
	int getId() const { return id; }
	void setVisionDistance(const double distance) { visionDistance = distance; }
	double getVisionDistance() const { return visionDistance; }
	void setVisionAngle(const int angle) { visionAngle = angle; }
	int getVisionAngle() const { return visionAngle; }
	void setHeading(std::pair<int, int> newHeading) { heading = newHeading; }
	std::pair<int, int> getHeading() const { return heading; }

	virtual void updateHealth() = 0;
	void changeHealth(const int healthToAdd) { health += healthToAdd; }

private:
	Enums::AgentType agentType;
	int id;
	int health;
	int visionAngle;
	double visionDistance;
	std::pair<int, int> heading;
};

