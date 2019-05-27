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
