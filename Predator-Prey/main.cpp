#include "Logger.h"

int main() {
	Logger::getInstance().Log("test", "Przyklad loggera");
	Logger::getInstance().Log("test", "Tak go uzywac");
	Logger::getInstance().Log("inny_log", "Pliki sa tworzone i dopisywane na koncu");

	int a = 15;

	Logger::getInstance().Log("inny_log", "Mozna przekazywac zmienne w stylu %d", a);
}
