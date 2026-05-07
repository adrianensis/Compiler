#include "Compiler/Core/Log/Log.hpp"

void Log::init(const std::string& logDirectory)
{
	logFile.open(logDirectory+smLogFile);
}

void Log::terminate()
{
	if(logFile.is_open())
	{
		logFile.close();
	}
}