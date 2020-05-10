/*
* RollingFileAppender.hh
*
* See the COPYING file for the terms of usage and distribution.
*/

#ifndef _LOG4CPP_ROLLINGFILEAPPENDER_HH
#define _LOG4CPP_ROLLINGFILEAPPENDER_HH

#include "fileappender.h"
#include <string>
#include <stdarg.h>

class RollingFileAppender : public FileAppender {
public:
	RollingFileAppender(const std::string& name,
		const std::string& fileName,
		size_t maxFileSize = 10 * 1024 * 1024,
		unsigned int maxBackupIndex = 1,
		bool append = true,
		mode_t mode = 00644);

	virtual void setMaxBackupIndex(unsigned int maxBackups);
	virtual unsigned int getMaxBackupIndex() const;
	virtual void setMaximumFileSize(size_t maxFileSize);
	virtual size_t getMaxFileSize() const;

	virtual void rollOver();

protected:
	virtual void doAppend(severity_level level, const char *file, int line, const std::string& message);

	unsigned int _maxBackupIndex;
	unsigned short int _maxBackupIndexWidth;	// keep constant index width by zeroing leading positions

	size_t _maxFileSize;
};


#endif // _LOG4CPP_ROLLINGFILEAPPENDER_HH
