#ifndef _FILEAPPENDER_HH
#define _FILEAPPENDER_HH

#include "appender.h"
#include <string>
#include <stdarg.h>


class FileAppender : public Appender {
public:

	FileAppender(const std::string& name, const std::string& fileName,
		bool append = true, mode_t mode = 00644);

	FileAppender(const std::string& name, int fd);
	virtual ~FileAppender();

	virtual bool reopen();

	virtual void close();

	virtual void setAppend(bool append);

	virtual bool getAppend() const;

	virtual void setMode(mode_t mode);

	virtual mode_t getMode() const;

	virtual void doAppend(severity_level level, const char *file, int line, const std::string& message);

	std::string now_time();
protected:
	

	const std::string _fileName;
	int _fd;
	int _flags;
	mode_t _mode;
};


#endif // _FILEAPPENDER_HH
