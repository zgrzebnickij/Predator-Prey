#pragma once
#include <utility>
#include <string>
#include <functional>
#include "Enums.h"

class Agent
{
public:
	Agent(Enums::AgentType agentType_,const int id_) :id(id_), agentType(agentType_), health(0), visionDistance(0), visionAngle(180) {}
	virtual ~Agent() = default;

	Enums::AgentType getAgentType() const { return agentType; }
	void setHealth(const int healthToSet) { health = healthToSet; }
	int getHealth() const { return health; }
	int getId() const { return id; }
	void setVisionDistance(const double distance) { visionDistance = distance; }
	double getVisionDistance() const { return visionDistance; }
	void setVisionAngle(const int angle) { visionAngle = angle; }
	int getVisionAngle() const { return visionAngle; }
	void setHeading(std::pair<int, int> newHeading) { heading = newHeading; }
	std::pair<int, int> getHeading() const { return heading; }
	void kill(std::function<void(int)> function) { function(id); }
	virtual void updateHealth() = 0;
	void changeHealth(const int healthToAdd) { health += healthToAdd; }

private:
	Enums::AgentType agentType;
	const int id;
	int health;
	double visionDistance;
	int visionAngle;
	std::pair<int, int> heading;
};

