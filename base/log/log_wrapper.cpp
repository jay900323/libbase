#include "log_wrapper.h"
#include "zx_utils.h"
#include "zx_str.h"
#include "appendersfactory.h"
#include <stdarg.h>

Log::Log(char *name, severity_level  level)
	:name(name),
	initialized(false),
	last_logged_time(0L),
	level(level),
	accessable(false),
	appender(FILE_APPENDER),
	log_max_size(5000000),
	log_max_num(10),
	log_line_num(false)
{

}

Log::~Log()
{
	removeAllAppenders();
}

/**
* @brief 日志等级是否有效.
*
* @param level : 日志等级
* @return bool,等级是否有效
*/
bool Log::isLogLevelValid(int level)
{
	switch (level)
	{
	case LEVEL_FATAL:
	case LEVEL_ERROR:
	case LEVEL_WARN:
	case LEVEL_INFO:
	case LEVEL_DEBUG:
	case LEVEL_ALL:
		return true;
		break;
	}

	return false;
}

int Log::setLogLevel(int leve)
{
	std::lock_guard<std::mutex> lock(log_mutex);

	if (!isLogLevelValid(leve))
	{
		return -1;
	}

	level = (severity_level)leve;
	return 0;
}

void	Log::debug(const char *file, int line, const char *fmt, ...)
{
	std::lock_guard<std::mutex> lock(log_mutex);

	if (LEVEL_DEBUG <= level) {
		va_list	args;
		va_start(args, fmt);
		log(LEVEL_DEBUG, file, line, fmt, args);
		va_end(args);
	}
}

void	Log::info(const char *file, int line, const char *fmt, ...)
{
	std::lock_guard<std::mutex> lock(log_mutex);

	if (LEVEL_INFO <= level) {
		va_list	args;
		va_start(args, fmt);
		log(LEVEL_INFO, file, line, fmt, args);
		va_end(args);
	}
}

void	Log::warn(const char *file, int line, const char *fmt, ...)
{
	std::lock_guard<std::mutex> lock(log_mutex);

	if (LEVEL_WARN <= level) {
		va_list	args;
		va_start(args, fmt);
		log(LEVEL_WARN, file, line, fmt, args);
		va_end(args);
	}
}

void	Log::error(const char *file, int line, const char *fmt, ...)
{
	std::lock_guard<std::mutex> lock(log_mutex);

	if (LEVEL_ERROR <= level) {
		va_list	args;
		va_start(args, fmt);
		log(LEVEL_ERROR, file, line, fmt, args);
		va_end(args);
	}
}

void	Log::fatal(const char *file, int line, const char *fmt, ...)
{
	std::lock_guard<std::mutex> lock(log_mutex);

	if (LEVEL_FATAL <= level) {
		va_list	args;
		va_start(args, fmt);
		log(LEVEL_FATAL, file, line, fmt, args);
		va_end(args);
	}
}

void Log::log(severity_level level, const char *file, int line, const char *fmt, va_list args)
{
	init_appender();

	static char log_message[MAX_BUFFER_LEN] = { 0 };
	z_vsnprintf(log_message, sizeof(log_message) - 1, fmt, args);
	callAppenders(level, log_line_num ?file:NULL, line, log_message);
}

void Log::addAppender(Appender* appender) {
	AppenderSet::iterator i = log_appenders.find(appender);
	if (log_appenders.end() == i) {
		log_appenders.insert(appender);
	}
}

void Log::callAppenders(severity_level level, const char *file, int line, const std::string& message) throw() {
	if (!log_appenders.empty()) {
		for (AppenderSet::const_iterator i = log_appenders.begin();
			i != log_appenders.end(); i++) {
			(*i)->doAppend(level, file, line, message);
		}
	}
}

void Log::removeAllAppenders() {
	for (AppenderSet::iterator i = log_appenders.begin();
		i != log_appenders.end(); i++) {
		delete (*i);
	}
	log_appenders.clear();
}

void Log::setLogLineNum(bool loglineNum)
{ 
	log_line_num = loglineNum;
}

void Log::set_log_appender(int append_mode)
{
	appender = append_mode;
}

void Log::setPath(const std::string &path)
{
	log_path = path;
}

int Log::getMaxSize()
{
	return log_max_size;
}

void Log::setMaxSize(int maxSize)
{
	log_max_size = maxSize;
}

int Log::getMaxNum()
{
	return log_max_num;
}

void Log::init_appender()
{
	if (!initialized) {
		if (appender & FILE_APPENDER) {
			std::string class_name = "rolling file";
			FactoryParams fp;
			std::ostringstream filename_stream;
			filename_stream << name << "_" << class_name;
			fp["name"] = filename_stream.str();
			fp["filename"] = log_path.c_str();
			fp["max_file_size"] = ZxBase::tostr<int>(log_max_size);
			fp["max_backup_index"] = ZxBase::tostr<int>(log_max_num);
			addAppender(AppendersFactory::getInstance().create(class_name, fp).release());
		}
		if (appender & CONSOLE_APPENDER)
		{
			std::string class_name = "console";
			FactoryParams fp;
			std::ostringstream filename_stream;
			filename_stream << name << "_" << class_name;
			fp["name"] = filename_stream.str();
			addAppender(AppendersFactory::getInstance().create(class_name, fp).release());
		}
		initialized = 1;
	}
}

void Log::setMaxNum(int maxNum)
{
	log_max_num = maxNum;
}

LogManage::LogManage()
	:root_logger(new Log("root", severity_level_t::LEVEL_INFO))
{
}

LogManage::~LogManage()
{

}

std::shared_ptr<Logger> LogManage::getLogger(std::string name/*="root"*/)
{
	std::lock_guard<std::mutex> lock(log_manage_mutex);
	if (name == "root") {
		return root_logger;
	}

	std::map< std::string, std::shared_ptr<Logger>>::iterator it = logger_dict.find(name);
	if (it != logger_dict.end()) {
		return it->second;
	}
	else {
		std::shared_ptr<Logger> logger(new Log(const_cast<char *>(name.data()), severity_level_t::LEVEL_INFO));
		logger_dict[name] = logger;
		return logger;
	}
}
