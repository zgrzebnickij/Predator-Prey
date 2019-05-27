#pragma once
#include <string>
#include <experimental/filesystem>

class Logger
{
private:
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
};

