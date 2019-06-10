#pragma once
#include <string>
#include <experimental/filesystem>
#include <map>
#include "Enums.h"

class Logger
{
private:
	using QuantityMap = std::map<Enums::AgentType, int>;
	Logger()
	{
		namespace fs = std::experimental::filesystem;

		const std::string logDirectory = "Logs/";
		const fs::path pathToLogs(logDirectory);
		if (!exists(pathToLogs)) {
			fs::create_directories(logDirectory);
		}
	}

public:
	static Logger& getInstance()
	{
		static Logger instance;
		return instance;
	}
	Logger(Logger const&) = delete;
	void operator=(Logger const&) = delete;

	void Log(const std::string& fileName, std::string message);
	void LogStep(QuantityMap statisticMap, const std::string& fileName, int step);
};

