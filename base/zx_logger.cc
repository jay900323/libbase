#include "zx_logger.h"
#include "log_wrapper.h"
#include <stdarg.h>

void	logging::debug(const char *file, int line, const char *fmt, ...)
{
	va_list	args;
	va_start(args, fmt);
	ZxSingleton<LogManage>::getInstance()->getLogger()->debug(file, line, fmt, args);
	va_end(args);
}

void	logging::info(const char *file, int line, const char *fmt, ...)
{
	va_list	args;
	va_start(args, fmt);
	ZxSingleton<LogManage>::getInstance()->getLogger()->info(file, line, fmt, args);
	va_end(args);
}

void	logging::warn(const char *file, int line, const char *fmt, ...)
{
	va_list	args;
	va_start(args, fmt);
	ZxSingleton<LogManage>::getInstance()->getLogger()->warn(file, line, fmt, args);
	va_end(args);
}

void	logging::error(const char *file, int line, const char *fmt, ...)
{
	va_list	args;
	va_start(args, fmt);
	ZxSingleton<LogManage>::getInstance()->getLogger()->error(file, line, fmt, args);
	va_end(args);
}

void	logging::fatal(const char *file, int line, const char *fmt, ...)
{
	va_list	args;
	va_start(args, fmt);
	ZxSingleton<LogManage>::getInstance()->getLogger()->fatal(file, line, fmt, args);
	va_end(args);
}

void logging::setPath(const std::string &path)
{
	ZxSingleton<LogManage>::getInstance()->getLogger()->setPath(path);
}

void logging::setLogLineNum(bool loglineNum)
{
	ZxSingleton<LogManage>::getInstance()->getLogger()->setLogLineNum(loglineNum);
}

void logging::setLogLevel(int leve)
{
	ZxSingleton<LogManage>::getInstance()->getLogger()->setLogLevel(leve);
}

void logging::setMaxSize(int maxSize)
{
	ZxSingleton<LogManage>::getInstance()->getLogger()->setMaxSize(maxSize);
}

void logging::setMaxNum(int maxNum)
{
	ZxSingleton<LogManage>::getInstance()->getLogger()->setMaxNum(maxNum);
}

void logging::set_log_appender(int append_mode)
{
	ZxSingleton<LogManage>::getInstance()->getLogger()->set_log_appender(append_mode);
}

const std::shared_ptr<Logger>& logging::getLogger(std::string name)
{
	return ZxSingleton<LogManage>::getInstance()->getLogger(name);
}

