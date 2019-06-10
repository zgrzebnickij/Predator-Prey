#include "Logger.h"
#include "Utilities.h"
#include <fstream>

void Logger::Log(const std::string& fileName, std::string message)
{
	std::ofstream logFile;
	const std::string logDirectory = "Logs/";
	const std::string completeFileName = logDirectory + fileName + ".log";

	logFile.open(completeFileName, std::ios::out | std::ios::app);
	if (logFile.is_open()) {
		logFile << Utils::CurrentDateTime() << ":\t";
		logFile << message << "\n";
		logFile.close();
	} else {
		std::cerr << "Cannot open log file!" << std::endl;
	}
}

void Logger::LogStep(QuantityMap statisticMap, const std::string& fileName, int step)
{
	std::ofstream logFile;
	const std::string logDirectory = "Logs/";
	const std::string completeFileName = logDirectory + fileName + ".log";

	logFile.open(completeFileName, std::ios::out | std::ios::app);
	if (logFile.is_open()) {
		logFile << step << ":\n";
		std::for_each(statisticMap.begin(), statisticMap.end(), [&logFile, &step](auto const& data) {
			logFile << Utils::AgentTypeToString.at(data.first) << "\t" << std::to_string(data.second) << std::endl;
		});
		logFile.close();
	}
	else {
		std::cerr << "Cannot open step_log file!" << std::endl;
	}
}
