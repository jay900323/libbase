#ifndef _CONSOLEAPPENDER_HH
#define _CONSOLEAPPENDER_HH

#include "appender.h"
#include <string>
#include <stdarg.h>


class ConsoleAppender : public Appender {
public:

	ConsoleAppender(const std::string& name);

	virtual ~ConsoleAppender();

	virtual bool reopen();

	virtual void close();

	virtual void doAppend(severity_level level, const char *file, int line, const std::string& message);
#ifdef _WIN32
	bool SetConsoleColor(WORD wAttributes);
	const WORD *colors();
	WORD getPriorityColor(int priority) throw();
#endif
	
protected:


	const std::string _fileName;
	int _fd;
	int _flags;
	mode_t _mode;
};


#endif // _CONSOLEAPPENDER_HH

