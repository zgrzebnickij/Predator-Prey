#include "RandomDevice.h"


int RandomDevice::getRandomInteger(const int range)
{
	return int(range * distributionGenerator(generatorEngine));
}

double RandomDevice::getProbability()
{
	return distributionGenerator(generatorEngine);
}
