#include <iostream>
#include "Logger.h"
#include "Environment.h"

int main() {
	auto env = Environment(6);
	env.showLattice();
	env.printAgents();
	env.nextStep();
	std::cout << "Hello world " << std::endl;
	Logger::getInstance().Log("test", "Przyklad loggera");
	Logger::getInstance().Log("test", "Tak go uzywac");
	Logger::getInstance().Log("inny_log", "Pliki sa tworzone i dopisywane na koncu");

	int a = 15;

	Logger::getInstance().Log("inny_log", "Mozna przekazywac zmienne w stylu %d", a);
}
