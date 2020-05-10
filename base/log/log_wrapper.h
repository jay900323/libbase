#ifndef _LOG_WRAPPER_H_
#define  _LOG_WRAPPER_H_
#include "appender.h"

class Log : public Logger
{
public:
	Log(char *name, severity_level  level);
	~Log();

	void init_appender();

	void addAppender(Appender* appender);
	void callAppenders(severity_level level, const char *file, int line, const std::string& message) throw();
	void removeAllAppenders();

	severity_level getLogLevel() const { return level; }
	void setLogLineNum(bool loglineNum);
	bool isLogLevelValid(int level);
	/**
	* @brief 设置日志等级.
	*
	* @param level 等级
	* @return      成功设置返回0，否则返回-1
	*/
	int setLogLevel(int leve);

	/**
	* @brief 设置文件路径
	*/
	void setPath(const std::string &path);
	/**
	* @brief 获取最大大小.
	*
	* @return int
	*/
	int getMaxSize();

	/**
	* @brief 设置最大大小.
	*
	* @param maxSize
	*/
	void setMaxSize(int maxSize);

	/**
	* @brief 获取最大个数.
	*
	* @return int
	*/
	int getMaxNum();

	/**
	* @brief 设置文件个数.
	*
	* @param maxNum
	*/
	void setMaxNum(int maxNum);

	void set_log_appender(int append_mode);

	void	debug(const char *file, int line, const char *fmt, ...);
	void	info(const char *file, int line, const char *fmt, ...);
	void	warn(const char *file, int line, const char *fmt, ...);
	void	error(const char *file, int line, const char *fmt, ...);
	void	fatal(const char *file, int line, const char *fmt, ...);
	void log(severity_level level, const char *file, int line, const char *fmt, va_list args);
	// void log(severity_level level, std::string &message);
public:
	/*日志名称*/
	char           *name;
	/*初始化标志*/
	bool       initialized;
	/*上次记录时间*/
	long            last_logged_time;
	/*记录等级*/
	severity_level  level;
	/*是否有权限访问*/
	bool accessable;
	/*输出方式*/
	int   appender;
	/*输出Appender集合*/
	AppenderSet log_appenders;
	std::mutex log_mutex;
	/*文件路径*/
	std::string log_path;
	/*最大日志文件大小*/
	int log_max_size;
	/*最大日志文件数目*/
	int log_max_num;
	bool log_line_num;
};

class BASE_CLASS LogManage
{
public:
	LogManage();
	~LogManage();
	std::shared_ptr<Logger> getLogger(std::string name = "root");
private:
	std::mutex log_manage_mutex;
	std::map < std::string, std::shared_ptr<Logger>> logger_dict;
	std::shared_ptr<Logger> root_logger;
};

#endif