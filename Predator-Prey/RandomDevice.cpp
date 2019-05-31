#include "RandomDevice.h"


int RandomDevice::getRandomInteger(const int range)
{
	return int(range * distributionGenerator(generatorEngine));
}

RandomDevice::Position RandomDevice::getRandomPosition(int range)
{
	return Position(getRandomInteger(range), getRandomInteger(range));
}

double RandomDevice::getProbability()
{
	return distributionGenerator(generatorEngine);
}
