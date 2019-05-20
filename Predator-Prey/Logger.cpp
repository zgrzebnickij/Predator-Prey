#include "Logger.h"
#include "Utilities.h"
#include <fstream>
#include <cstdarg>

void Logger::Log(const std::string& fileName, const char * format, ...)
{
	std::ofstream logFile;
	std::string fileNameWithExtension = fileName + ".log";
	logFile.open(fileNameWithExtension, std::ios::out | std::ios::app);
	if (logFile.is_open()) {
		char* sMessage = nullptr;
		int nLength = 0;
		va_list args;
		va_start(args, format);
		nLength = _vscprintf(format, args) + 1;
		sMessage = new char[nLength];
		vsprintf_s(sMessage, nLength, format, args);
		logFile << Util::CurrentDateTime() << ":\t";
		logFile << sMessage << "\n";
		va_end(args);
		delete[] sMessage;
		logFile.close();
	} else {
		std::cerr << "Cannot open log file!" << std::endl;
	}
}
