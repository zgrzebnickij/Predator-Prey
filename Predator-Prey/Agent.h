#pragma once
#include <utility>
#include <string>

class Agent
{
public:
	Agent(std::string typeOfAgent_) : typeOfAgent(typeOfAgent_), health(0), visionDistance(0), visionAngle(180) {}
	virtual ~Agent() = default;
	void setHealth(const int healthToSet) { health = healthToSet; }
	int getHealth() const { return health; }
	void setId(const int ID) { id = ID; }
	int getId() const { return id; }
	void setVisionDistance(const double distance) { visionDistance = distance; }
	double getVisionDistance() const { return visionDistance; }
	void setVisionAngle(const int angle) { visionAngle = angle; }
	int getVisionAngle() const { return visionAngle; }
	virtual void updateHealth() = 0;
	void changeHealth(const int healthToAdd) { health += healthToAdd; }
	void setHeading(std::pair<int, int> newHeading) { heading = newHeading; }
	std::pair<int, int> getHeading() const { return heading; }
	std::string getTypeOfAgent() const { return typeOfAgent; }
private:
	std::string typeOfAgent;
	int id;
	int health;
	double visionDistance;
	int visionAngle;
	std::pair<int, int> heading;
};

