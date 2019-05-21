#include "Logger.h"
#include "Utilities.h"
#include <fstream>
#include <cstdarg>
#include <experimental/filesystem>

void Logger::Log(const std::string& fileName, const char * message, ...)
{
	namespace fs = std::experimental::filesystem;
	std::ofstream logFile;
	const std::string logDirectory = "Logs/";
	std::string completeFileName = logDirectory + fileName + ".log";

	const fs::path pathToLogs(logDirectory);
	if(!exists(pathToLogs)) {
		fs::create_directories(logDirectory);
	}

	logFile.open(completeFileName, std::ios::out | std::ios::app);
	if (logFile.is_open()) {
		char* sMessage = nullptr;
		int nLength = 0;
		va_list args;
		va_start(args, message);
		nLength = _vscprintf(message, args) + 1;
		sMessage = new char[nLength];
		vsprintf_s(sMessage, nLength, message, args);
		logFile << Util::CurrentDateTime() << ":\t";
		logFile << sMessage << "\n";
		va_end(args);
		delete[] sMessage;
		logFile.close();
	} else {
		std::cerr << "Cannot open log file!" << std::endl;
	}
}
