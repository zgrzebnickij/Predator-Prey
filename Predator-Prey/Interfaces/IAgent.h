#pragma once
#include <utility>
#include <string>
#include <functional>
#include "Enums.h"

class Agent{
public:
	using Heading = std::pair<int, int>;
	using Callback = std::function<void(int)>;
	using AgentType = Enums::AgentType;

	Agent(const int ID_, const Enums::AgentType agentType_) : ID(ID_), agentType(agentType_), health(0), visionDistance(0), visionAngle(180) {}
	virtual ~Agent() = default;

	int getID() const { return ID; }
	AgentType getAgentType() const { return agentType; }
	void kill(Callback function) { function(ID); }

	void setHealth(const int healthToSet) { health = healthToSet; }
	int getHealth() const { return health; }
	void changeHealth(const int healthToAdd) { health += healthToAdd; }
	virtual void updateHealth() = 0;

	void setVisionDistance(const double distance) { visionDistance = distance; }
	double getVisionDistance() const { return visionDistance; }

	void setVisionAngle(const int angle) { visionAngle = angle; }
	int getVisionAngle() const { return visionAngle; }

	void setHeading(Heading newHeading) { heading = newHeading; }
	Heading getHeading() const { return heading; }


private:
	const int               ID;
	AgentType               agentType;
	int                     health;
	double                  visionDistance;
	int                     visionAngle;
	Heading                 heading;
};

