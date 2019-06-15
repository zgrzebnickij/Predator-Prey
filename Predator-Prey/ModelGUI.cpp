#include "ModelGUI.h"
#include "Logger.h"
#include "Enums.h"
#include <execution>
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <TGUI/TGUI.hpp>


ModelGUI::ModelGUI(const Matrix* matrix_, CheckTypeClbk checkType_, float windowHeight_, float windowWidth_, float latticeWidth_, ToggleSimClbk toggleSim_,
	CurrentStepClbk getCurrentStep_, GetParametersClbk getParameters_, SetParametersClbk setParameters_, ResetClbk reset_, GetQuantsClbk getQuants_,
	SetQuantsClbk setQuants_, GetItersClbk getIters_, SetItersClbk setIters_) :
	latticeMap(matrix_),
	scaleFactor(1.0f),
	windowHeight(windowHeight_),
	windowWidth(windowWidth_),
	latticeWidth(latticeWidth_),
	checkType(checkType_),
	toggleSim(toggleSim_),
	getCurrentStep(getCurrentStep_),
	getParameters(getParameters_),
	setParameters(setParameters_),
	reset(reset_),
	getQuants(getQuants_),
	setQuants(setQuants_),
	getIters(getIters_),
	setIters(setIters_),
	isRunning(false)
{
	if (!grassTexture.loadFromFile("Resources/Textures/field.png") || 
		!wolfTexture.loadFromFile("Resources/Textures/wolf.png") || 
		!sheepTexture.loadFromFile("Resources/Textures/sheep.png") ||
		!backgroundTexture.loadFromFile("Resources/Textures/background.png")) {
		Logger::getInstance().Log("errors", "Cannot load textures!");
	}

	grassTexture.setSmooth(true);
	wolfTexture.setSmooth(true);
	sheepTexture.setSmooth(true);
	backgroundTexture.setSmooth(true);
	grassSprite.setTexture(grassTexture);
	wolfSprite.setTexture(wolfTexture);
	sheepSprite.setTexture(sheepTexture);
	backgroundSprite.setTexture(backgroundTexture);

	startGUI();
}

ModelGUI::~ModelGUI()
{
	renderThread.join();
}

void ModelGUI::startGUI()
{
	scaleFactor = (latticeWidth / latticeMap->size()) / grassTexture.getSize().x;
	grassSprite.setScale(scaleFactor, scaleFactor);
	wolfSprite.setScale(scaleFactor, scaleFactor);
	sheepSprite.setScale(scaleFactor, scaleFactor);
	backgroundSprite.setPosition(800, 0);
	renderThread = std::thread([=] { renderingThread(); });
}

void ModelGUI::renderingThread()
{
	window.create(sf::VideoMode(windowWidth, windowHeight), "Predator-Prey Model");
	tgui::Gui gui{ window };
	window.setFramerateLimit(60);

	gui.loadWidgetsFromFile("Resources/GUI/form.txt");
	prepareWidgets(gui);
	downloadParameters();
	updateWidgets(gui);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			gui.handleEvent(event);
		}

		if (isRunning) {
			downloadParameters();
			updateWidgets(gui);
		}


		printLattice();

		window.draw(backgroundSprite);
		gui.draw();
		window.display();
	}
}

void ModelGUI::putAgents()
{
	for (auto row = 0; row < latticeMap->size(); row++) {
		for (auto col = 0; col < latticeMap->size(); col++) {
			switch (checkType(latticeMap->at(row).at(col))) {
			case Enums::AgentType::Predator:
				wolfSprite.setPosition(col * wolfTexture.getSize().x * scaleFactor, row * wolfTexture.getSize().y * scaleFactor);
				window.draw(wolfSprite);
				break;
			case Enums::AgentType::Prey:
				sheepSprite.setPosition(col * sheepTexture.getSize().x * scaleFactor, row * sheepTexture.getSize().y * scaleFactor);
				window.draw(sheepSprite);
				break;
			}
		}
	}
}

void ModelGUI::generateField()
{
	for (auto row = 0; row < latticeMap->size(); row++) {
		for (auto col = 0; col < latticeMap->size(); col++) {
			grassSprite.setPosition(col * grassTexture.getSize().x * scaleFactor, row * grassTexture.getSize().y * scaleFactor);
			window.draw(grassSprite);
		}
	}
}

void ModelGUI::printLattice()
{
	generateField();
	putAgents();
}

void ModelGUI::prepareWidgets(tgui::Gui& gui)
{
	tgui::Button::Ptr startButton = gui.get<tgui::Button>("startButton");
	startButton->connect("pressed", [&]() {
		toggleSim();
		if (!isRunning) { 
			uploadParameters(gui);
		} 
		isRunning = !isRunning;  
	});

	tgui::Button::Ptr resetButton = gui.get<tgui::Button>("resetButton");
	resetButton->connect("pressed", [&]() {
		toggleSim();
		uploadParameters(gui);
		reset();
		isRunning = false;
	});
}

void ModelGUI::updateWidgets(tgui::Gui & gui)
{
	tgui::TextBox::Ptr stepCounter = gui.get<tgui::TextBox>("stepCounter");
	stepCounter->setText(std::to_string(getCurrentStep()));

	tgui::TextBox::Ptr predatorMaxHPBox = gui.get<tgui::TextBox>("predatorMaxHPBox");
	predatorMaxHPBox->setText(std::to_string(parameters.predatorMaxHealth));
	tgui::TextBox::Ptr predatorMatingProbBox = gui.get<tgui::TextBox>("predatorMatingProbBox");
	std::stringstream predatorMatingProbStream;
	predatorMatingProbStream << std::fixed << std::setprecision(2) << parameters.predatorMatingProb;
	predatorMatingProbBox->setText(predatorMatingProbStream.str());
	tgui::TextBox::Ptr preyMaxHPBox = gui.get<tgui::TextBox>("preyMaxHPBox");
	preyMaxHPBox->setText(std::to_string(parameters.preyMaxHealth));
	tgui::TextBox::Ptr preyMatingProbBox = gui.get<tgui::TextBox>("preyMatingProbBox");
	std::stringstream preyMatingProbStream;
	preyMatingProbStream << std::fixed << std::setprecision(2) << parameters.preyMatingProb;
	preyMatingProbBox->setText(preyMatingProbStream.str());
	tgui::TextBox::Ptr preyHPToMateBox = gui.get<tgui::TextBox>("preyHPToMateBox");
	preyHPToMateBox->setText(std::to_string(parameters.preyHelthToMate));

	auto quants = getQuants();
	tgui::TextBox::Ptr predQuantBox = gui.get<tgui::TextBox>("predQuantBox");
	predQuantBox->setText(std::to_string(quants.at(Enums::AgentType::Predator)));
	tgui::TextBox::Ptr preyQuantBox = gui.get<tgui::TextBox>("preyQuantBox");
	preyQuantBox->setText(std::to_string(quants.at(Enums::AgentType::Prey)));

	tgui::TextBox::Ptr maxStepsBox = gui.get<tgui::TextBox>("maxStepsBox");
	maxStepsBox->setText(std::to_string(getIters()));
}

void ModelGUI::downloadParameters()
{
	parameters = getParameters();
}

void ModelGUI::uploadParameters(tgui::Gui& gui)
{
	tgui::TextBox::Ptr predatorMaxHPBox = gui.get<tgui::TextBox>("predatorMaxHPBox");
	int predatorMaxHP = std::stoi(predatorMaxHPBox->getText().toAnsiString());
	tgui::TextBox::Ptr predatorMatingProbBox = gui.get<tgui::TextBox>("predatorMatingProbBox");
	double predatorMatingProb = std::stod(predatorMatingProbBox->getText().toAnsiString());
	tgui::TextBox::Ptr preyMaxHPBox = gui.get<tgui::TextBox>("preyMaxHPBox");
	int preyMaxHP = std::stoi(preyMaxHPBox->getText().toAnsiString());
	tgui::TextBox::Ptr preyMatingProbBox = gui.get<tgui::TextBox>("preyMatingProbBox");
	double preyMatingProb = std::stod(preyMatingProbBox->getText().toAnsiString());
	tgui::TextBox::Ptr preyHPToMateBox = gui.get<tgui::TextBox>("preyHPToMateBox");
	int preyHPToMate = std::stoi(preyHPToMateBox->getText().toAnsiString());

	Utils::envData upload = { preyMatingProb , predatorMatingProb, preyMaxHP, preyHPToMate, predatorMaxHP, parameters.customModel };
	setParameters(upload);

	tgui::TextBox::Ptr predQuantBox = gui.get<tgui::TextBox>("predQuantBox");
	int predQuant = std::stoi(predQuantBox->getText().toAnsiString());
	tgui::TextBox::Ptr preyQuantBox = gui.get<tgui::TextBox>("preyQuantBox");
	int preyQuant = std::stoi(preyQuantBox->getText().toAnsiString());

	QuantityMap quants{
		{Enums::AgentType::Predator, predQuant},
		{ Enums::AgentType::Prey, preyQuant },
	};
	setQuants(quants);

	tgui::TextBox::Ptr maxStepsBox = gui.get<tgui::TextBox>("maxStepsBox");
	int maxSteps = std::stoi(maxStepsBox->getText().toAnsiString());
	setIters(maxSteps);
}
