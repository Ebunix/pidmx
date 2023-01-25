#pragma once
#include <vector>

#define CON_BLACK "\u001b[30m"
#define CON_RED "\u001b[31m"
#define CON_GREEN "\u001b[32m"
#define CON_YELLOW "\u001b[33m"
#define CON_BLUE "\u001b[34m"
#define CON_MAGENTA "\u001b[35m"
#define CON_CYAN "\u001b[36m"
#define CON_WHITE "\u001b[37m"
#define CON_BRIGHT_BLACK "\u001b[30;1m"
#define CON_BRIGHT_RED "\u001b[31;1m"
#define CON_BRIGHT_GREEN "\u001b[32;1m"
#define CON_BRIGHT_YELLOW "\u001b[33;1m"
#define CON_BRIGHT_BLUE "\u001b[34;1m"
#define CON_BRIGHT_MAGENTA "\u001b[35;1m"
#define CON_BRIGHT_CYAN "\u001b[36;1m"
#define CON_BRIGHT_WHITE "\u001b[37;1m"
#define CON_BG_BLACK "\u001b[40m"
#define CON_BG_RED "\u001b[41m"
#define CON_BG_GREEN "\u001b[42m"
#define CON_BG_YELLOW "\u001b[43m"
#define CON_BG_BLUE "\u001b[44m"
#define CON_BG_MAGENTA "\u001b[45m"
#define CON_BG_CYAN "\u001b[46m"
#define CON_BG_WHITE "\u001b[47m"
#define CON_BG_BRIGHT_BLACK "\u001b[40;1m"
#define CON_BG_BRIGHT_RED "\u001b[41;1m"
#define CON_BG_BRIGHT_GREEN "\u001b[42;1m"
#define CON_BG_BRIGHT_YELLOW "\u001b[43;1m"
#define CON_BG_BRIGHT_BLUE "\u001b[44;1m"
#define CON_BG_BRIGHT_MAGENTA "\u001b[45;1m"
#define CON_BG_BRIGHT_CYAN "\u001b[46;1m"
#define CON_BG_BRIGHT_WHITE "\u001b[47;1m"
#define CON_RESET "\u001b[0m"

float __logTime();

// #define LOG_ERROR_FORMAT(msg, ...)	printf(CON_BLACK "%.3f " CON_RED     "ERROR " CON_BLACK __FILE__ ":%i in " CON_BRIGHT_BLACK "%s" CON_BLACK ": " CON_BRIGHT_RED msg "\n" CON_RESET, __logTime(), __LINE__, __PRETTY_FUNCTION__, __VA_ARGS__)
// #define LOG_ERROR(msg)				printf(CON_BLACK "%.3f " CON_RED     "ERROR " CON_BLACK __FILE__ ":%i in " CON_BRIGHT_BLACK "%s" CON_BLACK ": " CON_BRIGHT_RED msg "\n" CON_RESET, __logTime(), __LINE__, __PRETTY_FUNCTION__)
// #define LOG_WARN_FORMAT(msg, ...)	printf(CON_BLACK "%.3f " CON_YELLOW  "WARN  " CON_BLACK __FILE__ ":%i in " CON_BRIGHT_BLACK "%s" CON_BLACK ": " CON_BRIGHT_YELLOW msg "\n" CON_RESET, __logTime(), __LINE__, __PRETTY_FUNCTION__, __VA_ARGS__)
// #define LOG_WARN(msg)				printf(CON_BLACK "%.3f " CON_YELLOW  "WARN  " CON_BLACK __FILE__ ":%i in " CON_BRIGHT_BLACK "%s" CON_BLACK ": " CON_BRIGHT_YELLOW msg "\n" CON_RESET, __logTime(), __LINE__, __PRETTY_FUNCTION__)
// #define LOG_FORMAT(msg, ...)		printf(CON_BLACK "%.3f " CON_BLACK   "MSG   " CON_BLACK __FILE__ ":%i: " CON_RESET msg "\n", __logTime(), __LINE__, __VA_ARGS__)
// #define LOG(msg)					printf(CON_BLACK "%.3f " CON_BLACK   "MSG   " CON_BLACK __FILE__ ":%i: " CON_RESET msg "\n", __logTime(), __LINE__)
// #define LOG_INFO_FORMAT(msg, ...)	printf(CON_BLACK "%.3f " CON_GREEN   "INFO  " CON_BLACK __FILE__ ":%i: " CON_GREEN msg "\n" CON_RESET, __logTime(), __LINE__, __VA_ARGS__)
// #define LOG_INFO(msg)				printf(CON_BLACK "%.3f " CON_GREEN   "INFO  " CON_BLACK __FILE__ ":%i: " CON_GREEN msg "\n" CON_RESET, __logTime(), __LINE__)
// #define LOG_DEBUG_FORMAT(msg, ...)	printf(CON_BLACK "%.3f " CON_MAGENTA "DEBUG " CON_BLACK __FILE__ ":%i: " CON_MAGENTA msg "\n" CON_RESET, __logTime(), __LINE__, __VA_ARGS__)
// #define LOG_DEBUG(msg)				printf(CON_BLACK "%.3f " CON_MAGENTA "DEBUG " CON_BLACK __FILE__ ":%i: " CON_MAGENTA msg "\n" CON_RESET, __logTime(), __LINE__)

#define LOG_ERROR_FORMAT(msg, ...)	printf(CON_BLACK "%.3f " CON_BRIGHT_RED     "ERROR " msg "\n" CON_RESET, __logTime(), __VA_ARGS__)
#define LOG_ERROR(msg)				printf(CON_BLACK "%.3f " CON_BRIGHT_RED     "ERROR " msg "\n" CON_RESET, __logTime())
#define LOG_WARN_FORMAT(msg, ...)	printf(CON_BLACK "%.3f " CON_BRIGHT_YELLOW  "WARN  " msg "\n" CON_RESET, __logTime(), __VA_ARGS__)
#define LOG_WARN(msg)				printf(CON_BLACK "%.3f " CON_BRIGHT_YELLOW  "WARN  " msg "\n" CON_RESET, __logTime())
#define LOG_FORMAT(msg, ...)		printf(CON_BLACK "%.3f " CON_BLACK   		"MSG   " msg "\n" CON_RESET, __logTime(), __VA_ARGS__)
#define LOG(msg)					printf(CON_BLACK "%.3f " CON_BLACK   		"MSG   " msg "\n" CON_RESET, __logTime())
#define LOG_INFO_FORMAT(msg, ...)	printf(CON_BLACK "%.3f " CON_GREEN   		"INFO  " msg "\n" CON_RESET, __logTime(), __VA_ARGS__)
#define LOG_INFO(msg)				printf(CON_BLACK "%.3f " CON_GREEN   		"INFO  " msg "\n" CON_RESET, __logTime())
#define LOG_DEBUG_FORMAT(msg, ...)	printf(CON_BLACK "%.3f " CON_MAGENTA 		"DEBUG " msg "\n" CON_RESET, __logTime(), __VA_ARGS__)
#define LOG_DEBUG(msg)				printf(CON_BLACK "%.3f " CON_MAGENTA 		"DEBUG " msg "\n" CON_RESET, __logTime())

#define LOG_ERROR_ISNULL(var)		LOG_ERROR_FORMAT("`%s` is null", #var)

enum ConsoleMessageType {
	ConsoleMessageType_Info,
	ConsoleMessageType_Debug,
	ConsoleMessageType_Warn,
	ConsoleMessageType_Error,
};

struct ConsoleMessage {
	const char* message;
	ConsoleMessageType type;
};

extern std::vector<ConsoleMessage> consolePanelLogBuffer;

void LogMessage(ConsoleMessageType type, const char* fmt, ...);
