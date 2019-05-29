#include "LatticePrinter.h"
#include "Logger.h"
#include "Enums.h"
#include <execution>
#include <iostream>


LatticePrinter::LatticePrinter(Matrix* matrix_)
	: latticeMap(matrix_)
	, scaleFactor(1)
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

	printLattice();
}

LatticePrinter::~LatticePrinter()
{
	renderThread.join();
}

void LatticePrinter::printLattice()
{
	scaleFactor = (800.0 / latticeMap->size()) / grassTexture.getSize().x;
	grassSprite.setScale(scaleFactor, scaleFactor);
	wolfSprite.setScale(scaleFactor, scaleFactor);
	sheepSprite.setScale(scaleFactor, scaleFactor);
	renderThread = std::thread([=] { renderingThread(); });
}

void LatticePrinter::renderingThread()
{
	window.create(sf::VideoMode(800, 800), "Predator-Prey Model");
	window.setFramerateLimit(60);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		generateField();
		putAgents();
	
		window.display();
	}
}

void LatticePrinter::putAgents()
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

void LatticePrinter::generateField()
{
	for (auto row = 0; row < latticeMap->size(); row++) {
		for (auto col = 0; col < latticeMap->size(); col++) {
			grassSprite.setPosition(col * grassTexture.getSize().x * scaleFactor, row * grassTexture.getSize().y * scaleFactor);
			window.draw(grassSprite);
		}
	}
}
