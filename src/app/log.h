#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <ctime>
#ifdef _WIN32
#include <Windows.h>
#endif /* _WIN32 */

namespace Logger
{
namespace Consts
{
#ifdef _WIN32
	const auto COLOR_ERR   = FOREGROUND_RED;
	const auto COLOR_INFO  = FOREGROUND_GREEN;
#define WIN32_SET_COLOR(hConsole, color) SetConsoleTextAttribute(hConsole, color);
#else
#ifndef DISABLE_COLOR_LOG
	const char* COLOR_ERR    = "\033[31m";
	const char* COLOR_INFO   = "\033[32m";
	const char* COLOR_DEBUG  = "\033[34m";
	const char* COLOR_STOP   = "\033[0m";
#else
	const char* COLOR_ERR    = "";
	const char* COLOR_INFO   = "";
	const char* COLOR_DEBUG  = "";
	const char* COLOR_STOP   = "";
#endif /* DISABLE_COLOR_LOG */
#endif /* _WIN32 */

	const int LEVEL_ERROR  = 1;
	const int LEVEL_INFO   = 2;
	const int LEVEL_DEBUG  = 3;
} /* namespace Consts */

inline void write_log(int level, const char* fmt, ...)
{
	if(!fmt)
	{
		return;
	}

	/* Add time as prefix */
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	std::stringstream ss;
	ss << std::put_time(std::localtime(&in_time_t), "%H:%M:%S");
	std::string log_str = "[" + ss.str() + "]";

	/* Set coloring */
#ifdef _WIN32
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	switch(level)
	{
		case Consts::LEVEL_ERROR:
		{
			WIN32_SET_COLOR(hConsole, Consts::COLOR_ERR);
			log_str.append("[ERROR]: ");
			break;
		}
		case Consts::LEVEL_INFO:
		{
			WIN32_SET_COLOR(hConsole, Consts::COLOR_INFO);
			log_str.append("[INFO]: ");
			break;
		}
		case Consts::LEVEL_DEBUG:
		{
			log_str.append("[DEBUG]: ");
			break;
		}
	}
#else
	switch(level)
	{
		case Consts::LEVEL_ERROR:
		{
			log_str.append("[" COLOR_ERR "ERROR" COLOR_STOP "]: ");
			break;
		}
		case Consts::LEVEL_INFO:
		{
			log_str.append("[" COLOR_INFO "INFO" COLOR_STOP "]: ");
			break;
		}
		case Consts::LEVEL_DEBUG:
		{
			log_str.append("[" COLOR_DEBUG "DEBUG" COLOR_STOP "]: ");
			break;
		}
	}
#endif /* _WIN32 */

	va_list args;
	va_start(args, fmt);

	/* TODO: Write to a file, but on linux all colors should be ommitted */
	log_str += std::string(fmt) + "\n";
	vprintf(log_str.c_str(), args);

	va_end(args);
}
} /* namespace Logger */

/* TODO: Hide debug logs when built for release, OR add 'levels' of logging to write_log function */
#define LOG_DEBUG(fmt, ...) do { Logger::write_log(Logger::Consts::LEVEL_DEBUG, "%s - " fmt, __FUNCTION__, ##__VA_ARGS__); } while(false)
#define LOG_ERROR(fmt, ...) do { Logger::write_log(Logger::Consts::LEVEL_ERROR, "%s - " fmt, __FUNCTION__, ##__VA_ARGS__); } while(false)
#define LOG_INFO(fmt, ...) do { Logger::write_log(Logger::Consts::LEVEL_INFO, "%s - " fmt, __FUNCTION__, ##__VA_ARGS__); } while(false)