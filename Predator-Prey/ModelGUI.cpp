#include "ModelGUI.h"
#include "Logger.h"
#include "Enums.h"
#include <execution>
#include <TGUI/TGUI.hpp>


ModelGUI::ModelGUI(const Matrix* matrix_, float windowHeight_, float windowWidth_, float latticeWidth_) :
	latticeMap(matrix_),
	scaleFactor(1.0f),
	windowHeight(windowHeight_),
	windowWidth(windowWidth_),
	latticeWidth(latticeWidth_)
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

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			gui.handleEvent(event);
		}
		printLattice();

		gui.draw();
		window.display();
	}
}

void ModelGUI::putAgents()
{
	for (auto row = 0; row < latticeMap->size(); row++) {
		for (auto col = 0; col < latticeMap->size(); col++) {
			switch (static_cast<Enums::AgentType>(latticeMap->at(row).at(col))) {
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
