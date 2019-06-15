#include "ModelGUI.h"
#include "Logger.h"
#include "Enums.h"
#include <execution>
#include <iostream>
#include <TGUI/TGUI.hpp>


ModelGUI::ModelGUI(const Matrix* matrix_, CheckTypeClbk checkType_, float windowHeight_, float windowWidth_, float latticeWidth_, ToggleSimClbk toggleSim_,
	CurrentStepClbk getCurrentStep_) :
	latticeMap(matrix_),
	scaleFactor(1.0f),
	windowHeight(windowHeight_),
	windowWidth(windowWidth_),
	latticeWidth(latticeWidth_),
	checkType(checkType_),
	toggleSim(toggleSim_),
	getCurrentStep(getCurrentStep_)
{
	if (!grassTexture.loadFromFile("Resources/Textures/field.png") || 
		!wolfTexture.loadFromFile("Resources/Textures/wolf.png") || 
		!sheepTexture.loadFromFile("Resources/Textures/sheep.png")) {
		Logger::getInstance().Log("errors", "Cannot load textures!");
	}

	grassTexture.setSmooth(true);
	wolfTexture.setSmooth(true);
	sheepTexture.setSmooth(true);
	grassSprite.setTexture(grassTexture);
	wolfSprite.setTexture(wolfTexture);
	sheepSprite.setTexture(sheepTexture);

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
	renderThread = std::thread([=] { renderingThread(); });
}

void ModelGUI::renderingThread()
{
	window.create(sf::VideoMode(windowWidth, windowHeight), "Predator-Prey Model");
	tgui::Gui gui{ window };
	window.setFramerateLimit(60);

	gui.loadWidgetsFromFile("Resources/GUI/form.txt");
	prepareWidgets(gui);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			gui.handleEvent(event);
		}

		tgui::TextBox::Ptr stepCounter = gui.get<tgui::TextBox>("stepCounter");
		stepCounter->setText(std::to_string(getCurrentStep()));

		printLattice();

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
	startButton->connect("pressed", [&]() {	toggleSim(); });
}
