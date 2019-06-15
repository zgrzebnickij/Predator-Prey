#pragma once
#include <TGUI/TGUI.hpp>
#include <thread>
#include "Utilities.h"
#include "Interfaces/IAgent.h"

class ModelGUI
{
	using Matrix = std::vector<std::vector<int>>;
	using AgentVec = std::vector<std::unique_ptr<Agent>>;
	using QuantityMap = std::map<Enums::AgentType, int>;

	using CheckTypeClbk = std::function<Enums::AgentType(int)>;
	using ToggleSimClbk = std::function<void()>;
	using CurrentStepClbk = std::function<int()>;
	using GetParametersClbk = std::function<Utils::envData()>;
	using SetParametersClbk = std::function<void(Utils::envData)>;
	using ResetClbk = std::function<void()>;
	using GetQuantsClbk = std::function<QuantityMap()>;
	using SetQuantsClbk = std::function<void(QuantityMap)>;
	using GetItersClbk = std::function<int()>;
	using SetItersClbk = std::function<void(int)>;

public:
	ModelGUI(const Matrix* matrix_, CheckTypeClbk checkType_, float windowHeight_, float windowWidth_, float latticeWidth_, ToggleSimClbk toggleSim_,
		CurrentStepClbk getCurrentStep_, GetParametersClbk getParameters_, SetParametersClbk setParameters_, ResetClbk reset_, GetQuantsClbk getQuants_,
		SetQuantsClbk setQuants_, GetItersClbk getIters_, SetItersClbk setIters_);
	~ModelGUI();

private:
	void startGUI();
	void renderingThread();
	void generateField();
	void putAgents();
	void printLattice();
	void prepareWidgets(tgui::Gui& gui);
	void updateWidgets(tgui::Gui& gui);
	void downloadParameters();
	void uploadParameters(tgui::Gui& gui);

	sf::RenderWindow window;
	std::thread renderThread;
	const Matrix* latticeMap;
	CheckTypeClbk checkType;
	ToggleSimClbk toggleSim;
	CurrentStepClbk getCurrentStep;
	GetParametersClbk getParameters;
	SetParametersClbk setParameters;
	ResetClbk reset;
	GetQuantsClbk getQuants;
	SetQuantsClbk setQuants;
	GetItersClbk getIters;
	SetItersClbk setIters;

	float scaleFactor;
	float windowHeight;
	float windowWidth;
	float latticeWidth;
	bool isRunning;

	sf::Texture grassTexture;
	sf::Texture wolfTexture;
	sf::Texture sheepTexture;
	sf::Sprite grassSprite;
	sf::Sprite wolfSprite;
	sf::Sprite sheepSprite;
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;

	Utils::envData parameters;
};

