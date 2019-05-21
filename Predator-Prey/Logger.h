#pragma once
#include <string>

class Logger
{
private:
	Logger() {}

public:
	static Logger& getInstance()
	{
		static Logger instance;
		return instance;
	}
	Logger(Logger const&) = delete;
	void operator=(Logger const&) = delete;

	void Log(const std::string& fileName, const char * format, ...);
};

