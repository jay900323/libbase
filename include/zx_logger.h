#ifndef _LOGGER_ENTRY_H
#define _LOGGER_ENTRY_H

#include <mutex>
#include "zx_base.h"
#include "zx_singleton.h"

typedef enum log_appender_t {
	FILE_APPENDER = 1 << 0,
	SYSLOG_APPENDER = 1 << 1,
	CONSOLE_APPENDER = 1 << 2,
} log_appender;

typedef enum severity_level_t {
	LEVEL_FATAL = 0,
	LEVEL_ERROR = 1,
	LEVEL_WARN = 2,
	LEVEL_INFO = 3,
	LEVEL_DEBUG = 4,
	LEVEL_ALL = 5
} severity_level;

class Logger
{
public:
	virtual int setLogLevel(int leve) = 0;
	virtual void setLogLineNum(bool loglineNum) = 0;
	virtual void set_log_appender(int append_mode) = 0;
	virtual void setPath(const std::string &path) = 0;
	virtual void setMaxSize(int maxSize) = 0;
	virtual void setMaxNum(int maxNum) = 0;
	virtual void	debug(const char *file, int line, const char *fmt, ...) = 0;
	virtual void	info(const char *file, int line, const char *fmt, ...) = 0;
	virtual void	warn(const char *file, int line, const char *fmt, ...) = 0;
	virtual void	error(const char *file, int line, const char *fmt, ...) = 0;
	virtual void	fatal(const char *file, int line, const char *fmt, ...) = 0;
	virtual void log(severity_level level, const char *file, int line, const char *fmt, va_list args) = 0;
};

class BASE_CLASS logging {
public:

	static void debug(const char *file, int line, const char *fmt, ...);
	static void info(const char *file, int line, const char *fmt, ...);
	static void warn(const char *file, int line, const char *fmt, ...);
	static void error(const char *file, int line, const char *fmt, ...);
	static void fatal(const char *file, int line, const char *fmt, ...);
	static void setPath(const std::string &path);
	static void setLogLineNum(bool loglineNum);
	static void setLogLevel(int leve);
	static void setMaxSize(int maxSize);
	static void setMaxNum(int maxNum);
	static void set_log_appender(int append_mode);
	static const std::shared_ptr<Logger>& getLogger(std::string name);
};

#define LOG_DEBUG(fmt, ...)  logging::debug(__FILE__,   __LINE__, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  logging::info(__FILE__,   __LINE__, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  logging::warn(__FILE__,   __LINE__, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...)  logging::error(__FILE__,   __LINE__, fmt, ##__VA_ARGS__)
#define LOG_FATAL(fmt, ...)  logging::fatal(__FILE__,   __LINE__, fmt, ##__VA_ARGS__)

#define VLOG_DEBUG(logger, fmt, ...)  logger->debug(__FILE__,   __LINE__, fmt, ##__VA_ARGS__)
#define VLOG_INFO(logger, fmt, ...)  logger->info(__FILE__,   __LINE__, fmt, ##__VA_ARGS__)
#define VLOG_WARN(logger, fmt, ...)  logger->warn(__FILE__,   __LINE__, fmt, ##__VA_ARGS__)
#define VLOG_ERROR(logger, fmt, ...)  logger->error(__FILE__,   __LINE__, fmt, ##__VA_ARGS__)
#define VLOG_FATAL(logger, fmt, ...)  logger->fatal(__FILE__,   __LINE__, fmt, ##__VA_ARGS__)
#endif