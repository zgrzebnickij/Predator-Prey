#include <iostream>
#include "Prey.h"
#include "Logger.h"

int main() {
	std::cout << "Hello world" << std::endl;
	auto prey = Prey();
	Logger::getInstance().Log("test", "Przyklad loggera");
	Logger::getInstance().Log("test", "Tak go uzywac");
	Logger::getInstance().Log("inny_log", "Pliki sa tworzone i dopisywane na koncu");

	int a = 15;

	Logger::getInstance().Log("inny_log", "Mozna przekazywac zmienne w stylu %d", a);
}
