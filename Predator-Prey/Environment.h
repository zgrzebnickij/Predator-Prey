#pragma once
#include "Interfaces/IAgent.h"
#include "Lattice.h"
#include "ModelGUI.h"
#include <vector>
#include <string>
#include <functional>


class Environment
{
public:
	using Position = std::pair<int, int>;
	using Heading = std::pair<int, int>;
	using PositionsVec = std::vector<Position>;
	using AgentType = Enums::AgentType;
	using QuantityMap = std::map<AgentType, int>;

	Environment(int lattSize_, QuantityMap qMap_, const int predatorMaxHealth_, const int preyHelthToMate_, const int numberOfIterations_, bool blindAgents_ = true);

	void nextStep();

	void checkNeighbours(Position agentPosition);
	PositionsVec neighboursFromRange(double visionAngle, double visionRange, Heading heading);

	void blindAgentTurn(Position agentPosition);
	void sightedAgentTurn(Position agentPosition);
	void mating(Position agentPosition);
	void finishTurn();
	void makeIterations();
	Position generateMovePosition(Position position);

	int getPreyMaxHealth() { return preyMaxHealth; }
	void setPreyMaxHealth(const int maxHealth) {preyMaxHealth = maxHealth;}

	double getPreyMatingProb() { return preyMatingProb; }
	void setPreyMatingProb(const double matingProb) { preyMatingProb = matingProb; }

	double getPredatorMatingProb() { return predatorMatingProb; }
	void setPredatorMatingProb(const double matingProb) { predatorMatingProb = matingProb; }

	int getPreyHelthToMate() { return preyHelthToMate; }
	void setPreyHelthToMate(const int matingAge) { preyHelthToMate = matingAge; }

	int getPredatorMaxHealth() { return predatorMaxHealth; }
	void setPredatorMaxHealth(const int maxHealth) { predatorMaxHealth = maxHealth; }

	Utils::envData getParameters() {
		Utils::envData parameters = {
			preyMatingProb,
			predatorMatingProb,
			preyMaxHealth,
			preyHelthToMate,
			predatorMaxHealth,
			customModel
		};
		return parameters;
	}
	void setParameters(Utils::envData data) {
		preyMatingProb = data.preyMatingProb;
		predatorMatingProb = data.predatorMatingProb;
		preyMaxHealth = data.preyMaxHealth;
		preyHelthToMate = data.preyHelthToMate;
		predatorMaxHealth = data.predatorMaxHealth;
		customModel = data.customModel;
	}


private:
	//prob version
	double preyMatingProb;
	int preyMaxHealth;
	double predatorMatingProb;
	//
	int numberOfIterations;
	int preyHelthToMate;
	int predatorMaxHealth;
	int latticeSize;
	bool customModel;
	QuantityMap qMap;
	std::shared_ptr<void(std::pair<int, int>)> agentTurn;
	std::shared_ptr<ILattice> lattice;
	std::shared_ptr<ModelGUI> gui;
};

