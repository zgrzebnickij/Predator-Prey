#pragma once
#include <SFML/Graphics.hpp>
#include <thread>

class LatticePrinter
{
	using Matrix = std::vector<std::vector<int>>;
public:
	LatticePrinter(Matrix* matrix_);
	~LatticePrinter();

private:
	void printLattice();
	void renderingThread();
	void generateField();
	void putAgents();

	sf::RenderWindow window;
	std::thread renderThread;
	Matrix* latticeMap;

	double scaleFactor;

	sf::Texture grassTexture;
	sf::Texture wolfTexture;
	sf::Texture sheepTexture;
	sf::Sprite grassSprite;
	sf::Sprite wolfSprite;
	sf::Sprite sheepSprite;
};

