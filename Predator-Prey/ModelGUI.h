#pragma once
#include <TGUI/TGUI.hpp>
#include <thread>
#include "Interfaces/IAgent.h"

class ModelGUI
{
	using Matrix = std::vector<std::vector<int>>;
	using AgentVec = std::vector<std::unique_ptr<Agent>>;
	using CheckTypeClbk = std::function<Enums::AgentType(int)>;
	using ToggleSimClbk = std::function<void()>;
	using CurrentStepClbk = std::function<int()>;

public:
	ModelGUI(const Matrix* matrix_, CheckTypeClbk checkType_, float windowHeight_, float windowWidth_, float latticeWidth_, ToggleSimClbk toggleSim_,
		CurrentStepClbk getCurrentStep_);
	~ModelGUI();

private:
	void startGUI();
	void renderingThread();
	void generateField();
	void putAgents();
	void printLattice();
	void prepareWidgets(tgui::Gui& gui);

	sf::RenderWindow window;
	std::thread renderThread;
	const Matrix* latticeMap;
	CheckTypeClbk checkType;
	ToggleSimClbk toggleSim;
	CurrentStepClbk getCurrentStep;

	float scaleFactor;
	float windowHeight;
	float windowWidth;
	float latticeWidth;

	sf::Texture grassTexture;
	sf::Texture wolfTexture;
	sf::Texture sheepTexture;
	sf::Sprite grassSprite;
	sf::Sprite wolfSprite;
	sf::Sprite sheepSprite;
};

