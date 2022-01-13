#include "logger.h"

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdarg.h>

Logger *Logger::instance;

void Logger::init(const std::string &title, const std::string &version, const bool &exportToFile, const std::string &directory) {

	if (ENABLE_LOG && !instance) { instance = new Logger(title, version, exportToFile, directory); }

}

Logger * const Logger::getInstance() {

	return instance;

}

Logger::Logger(const std::string &title, const std::string &version, const bool &exportToFile, const std::string &directory) {

	this->title = title;
	this->version = version;

	previousError = Logger::LogType::NONE;

	this->exportToFile = exportToFile;

	if (exportToFile) {

		this->directory = directory;

		logFileRead = std::ifstream(directory.c_str());
		logFileWrite = std::ofstream(directory.c_str());

		initFile();

	}

	std::cout << "Logger ready" << std::endl;

}

Logger::~Logger() {

	std::cout << "\nLogger system shut down" << std::endl;

	if (exportToFile) {

		logFileWrite.open(directory.c_str(), std::ios::app);

		logFileWrite << "\nPlease make a copy of this file you wish to save this file";
		logFileWrite << "\nThe next runtime of the application will re-write the contents of this file." << std::endl;

		std::cout << "All logs dumpped at: " << directory.c_str() << std::endl;

		logFileWrite.close();

	}

	delete[] formattedMsg;

}

void Logger::initFile() {

	//bool fileExist = logFileRead.is_open();

	std::remove(directory.c_str());

	logFileWrite.open(directory.c_str(), std::ios::app);

	logFileWrite << "\n" << std::endl;

	logFileWrite.close();

	logFileWrite.open(directory.c_str(), std::ios::app);

	logFileWrite << title << "   v" << version << std::endl;

	logFileWrite.close();

}

std::string Logger::getDateTime() {

   // https://stackoverflow.com/questions/17223096/outputting-date-and-time-in-c-using-stdchrono //

	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	std::stringstream ss;
	ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");

	return ss.str();

}

void Logger::writeToFile(const char* header, const char* msg, const bool &differentLog) {

	logFileWrite.open(directory.c_str(), std::ios::app);

	if (differentLog) { logFileWrite << "\n"; }

	logFileWrite << "[" << header << "] " << getDateTime() << ":	";

	logFileWrite << msg << std::endl;

	logFileWrite.close();

}

void Logger::writeLog(const char* logMsg, ...) {

	if (!ENABLE_LOG || instance == nullptr) { return; }

	bool differentLog = previousError != Logger::LogType::LOG;

	previousError = Logger::LogType::LOG;

	va_start(args, logMsg);
	vsprintf(formattedMsg, logMsg, args);
	va_end(args);

	if (exportToFile) { writeToFile("LOG", formattedMsg, differentLog); }

	if (differentLog) { std::cout << "\n"; }

	std::cout << "[LOG] " << formattedMsg << std::endl;

}

void Logger::infoLog(const char* infoMsg, ...) {

	if (!ENABLE_LOG || instance == nullptr) { return; }

	bool differentLog = previousError != Logger::LogType::INFO;

	previousError = Logger::LogType::INFO;

	va_start(args, infoMsg);
	vsprintf(formattedMsg, infoMsg, args);
	va_end(args);

	if (exportToFile) { writeToFile("INFO", formattedMsg, differentLog); }

	if (differentLog) { std::cout << "\n"; }

	std::cout << "[INFO] " << formattedMsg << std::endl;

}

void Logger::debugLog(const char* dbugMsg, ...) {

	if (!ENABLE_LOG || instance == nullptr) { return; }

	bool differentLog = previousError != Logger::LogType::DEBUG;

	previousError = Logger::LogType::DEBUG;

	va_start(args, dbugMsg);
	vsprintf(formattedMsg, dbugMsg, args);
	va_end(args);

	if (exportToFile) { writeToFile("DEBUG", formattedMsg, differentLog); }

	if (differentLog) { std::cout << "\n"; }

	std::cout << "[DEBUG] " << formattedMsg << std::endl;

}

void Logger::warningLog(const char* wrngMsg, ...) {

	if (!ENABLE_LOG || instance == nullptr) { return; }

	bool differentLog = previousError != Logger::LogType::WARNING;

	previousError = Logger::LogType::WARNING;

	va_start(args, wrngMsg);
	vsprintf(formattedMsg, wrngMsg, args);
	va_end(args);

	if (exportToFile) { writeToFile("WARNING", formattedMsg, differentLog); }

	if (differentLog) { std::cout << "\n"; }

	std::cerr << "[WARNING] " << formattedMsg << std::endl;

}

void Logger::errorLog(const char* errMsg, ...) {

	if (!ENABLE_LOG || instance == nullptr) { return; }

	bool differentLog = previousError != Logger::LogType::ERROR;

	previousError = Logger::LogType::ERROR;

	va_start(args, errMsg);
	vsprintf(formattedMsg, errMsg, args);
	va_end(args);

	if (exportToFile) { writeToFile("ERROR", formattedMsg, differentLog); }

	if (differentLog) { std::cout << "\n"; }

	std::cerr << "[ERROR] " << formattedMsg << std::endl;

}

void Logger::customLog(const char* header, const char* msg, ...) {

	if (!ENABLE_LOG || instance == nullptr) { return; }

	bool differentLog = previousError != Logger::LogType::CUSTOM;

	previousError = Logger::LogType::CUSTOM;

	va_start(args, msg);
	vsprintf(formattedMsg, msg, args);
	va_end(args);

	if (exportToFile) { writeToFile(header, formattedMsg, differentLog); }

	if (differentLog) { std::cout << "\n"; }

	std::cerr << "[" << header << "] " << formattedMsg << std::endl;

}
