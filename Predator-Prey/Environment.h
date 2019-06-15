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
	int getCurrentStep();
private:
	void toggleSimulationRun();

	double preyMatingProb;
	int preyMaxHealth;
	double predatorMatingProb;
	int numberOfIterations;
	int preyHelthToMate;
	int predatorMaxHealth;
	int latticeSize;
	int currentStep;
	bool blindAgents;
	bool isSimulationRunning;
	std::string stepLogFileName;
	QuantityMap qMap;
	
	std::shared_ptr<void(std::pair<int, int>)> agentTurn;
	std::shared_ptr<ILattice> lattice;
	std::shared_ptr<ModelGUI> gui;
};

