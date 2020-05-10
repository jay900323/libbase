#ifndef _LOG4CPP_APPENDER_HH
#define _LOG4CPP_APPENDER_HH
#ifdef _WIN32
#include "MSThreads.h"
#else
#include "PThreads.h"
#endif
#include <string>
#include <map>
#include <vector>
#include <set>
#include "zx_logger.h"

#ifdef _WIN32
typedef int mode_t;
#endif
class Appender
{
public:
	static Appender* getAppender(const std::string& name);
	static bool reopenAll();
	static void closeAll();
protected:
	/*将构造函数设置为保护的允许子类调用 但是不能通过new构造*/
	Appender(const std::string& name);
	const std::string *names();
	const std::string& getPriorityName(int priority) throw();
public:
	virtual ~Appender();
	virtual void doAppend(severity_level level, const char *file, int line, const std::string &message) = 0;
	virtual bool reopen() = 0;
	virtual void close() = 0;
	inline const std::string& getName() const { return _name; };
private:
	typedef std::map<std::string, Appender*> AppenderMap;

	static AppenderMap& _getAllAppenders();
	static void _deleteAllAppenders();
	static void _deleteAllAppendersWOLock(std::vector<Appender*> &appenders);
	static void _addAppender(Appender* appender);
	static void _removeAppender(Appender* appender);

	const std::string _name;
public:
	class AppenderMapStorage {
	public:
		Appender::AppenderMap* _allAppenders;	// single shared instance, nifty-counter defensed
		threading::Mutex _appenderMapMutex;	// mutex protecting map from multiple thread access 

		AppenderMapStorage();
		~AppenderMapStorage();
	};
	class AppenderMapStorageInitializer {
	public:
		AppenderMapStorageInitializer();
		~AppenderMapStorageInitializer();
	};
private:
	static AppenderMapStorage &_appenderMapStorageInstance;
};

static Appender::AppenderMapStorageInitializer appenderMapStorageInitializer; // static initializer for every translation unit
typedef std::set<Appender *> AppenderSet;

#endif
