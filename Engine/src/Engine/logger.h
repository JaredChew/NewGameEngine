#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <fstream>

#define ENABLE_LOG false

#ifdef _DEBUG

#undef  ENABLE_LOG
#define ENABLE_LOG true

#endif //_DEBUG

#define MESSAGE_BUFFER 4096

/*

		Alternative

	1 log function with enum to switch between logging type

*/

class Logger {

private:

	static Logger *instance;

	std::string title;
	std::string version;
	std::string directory;

	std::ifstream logFileRead;
	std::ofstream logFileWrite;

	va_list args;

	char formattedMsg[MESSAGE_BUFFER];

	bool exportToFile;

	enum class LogType { NONE, LOG, INFO, DEBUG, WARNING, ERROR, CUSTOM } previousError;

private:

	Logger(const std::string& title, const std::string& version, const bool &exportToFile, const std::string &directory);
	~Logger();

	void initFile();

	std::string getDateTime();

	void writeToFile(const char* header, const char* msg, const bool &differentLog);

public:

	static void init(const std::string &title, const std::string &version, const bool &exportToFile, const std::string &directory = "log_export.txt");
	static Logger* const getInstance();

	void writeLog(const char* logMsg, ...);
	void infoLog(const char* infoMsg, ...);
	void debugLog(const char* dbugMsg, ...);
	void warningLog(const char* wrngMsg, ...);
	void errorLog(const char* errMsg, ...);
	void customLog(const char* header, const char* msg, ...);

};