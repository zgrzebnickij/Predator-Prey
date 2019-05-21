#pragma once
#include <random>

class RandomDevice
{
private:
	RandomDevice(): generatorEngine(randomDevice()), distributionGenerator(0.0, 1.0) {}

public:
	static RandomDevice& getInstance()
	{
		static RandomDevice instance;
		return instance;
	}
	RandomDevice(RandomDevice const&) = delete;
	void operator=(RandomDevice const&) = delete;

	int getRandomInteger(int range);
	double getProbability();

private:
	std::random_device randomDevice;
	std::mt19937_64 generatorEngine;
	std::uniform_real_distribution<double> distributionGenerator;
};

