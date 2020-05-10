/*
* RollingFileAppender.cpp
*
* See the COPYING file for the terms of usage and distribution.
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "rollingfileappender.h"
#include "factoryparams.h"
#include <memory>
#include <stdio.h>
#include <math.h>
#include <iomanip>
#ifdef _WIN32
#include <io.h>
#endif

#pragma warning(disable:4996)

RollingFileAppender::RollingFileAppender(const std::string& name,
	const std::string& fileName,
	size_t maxFileSize,
	unsigned int maxBackupIndex,
	bool append,
	mode_t mode) :
	FileAppender(name, fileName, append, mode),
	_maxBackupIndex(maxBackupIndex > 0 ? maxBackupIndex : 1),
	_maxBackupIndexWidth((_maxBackupIndex > 0) ? log10((float)_maxBackupIndex) + 1 : 1),
	_maxFileSize(maxFileSize) {
}

void RollingFileAppender::setMaxBackupIndex(unsigned int maxBackups) {
	_maxBackupIndex = maxBackups;
	_maxBackupIndexWidth = (_maxBackupIndex > 0) ? log10((float)_maxBackupIndex) + 1 : 1;
}

unsigned int RollingFileAppender::getMaxBackupIndex() const {
	return _maxBackupIndex;
}

void RollingFileAppender::setMaximumFileSize(size_t maxFileSize) {
	_maxFileSize = maxFileSize;
}

size_t RollingFileAppender::getMaxFileSize() const {
	return _maxFileSize;
}

void RollingFileAppender::rollOver() {
	::close(_fd);
	if (_maxBackupIndex > 0) {
		std::ostringstream filename_stream;
		filename_stream << _fileName << "." << std::setw(_maxBackupIndexWidth) << std::setfill('0') << _maxBackupIndex << std::ends;
		// remove the very last (oldest) file
		std::string last_log_filename = filename_stream.str();
		// std::cout << last_log_filename << std::endl; // removed by request on sf.net #140
		::remove(last_log_filename.c_str());

		// rename each existing file to the consequent one
		for (unsigned int i = _maxBackupIndex; i > 1; i--) {
			filename_stream.str(std::string());
			filename_stream << _fileName << '.' << std::setw(_maxBackupIndexWidth) << std::setfill('0') << i - 1 << std::ends;  // set padding so the files are listed in order
			::rename(filename_stream.str().c_str(), last_log_filename.c_str());
			last_log_filename = filename_stream.str();
		}
		// new file will be numbered 1
		::rename(_fileName.c_str(), last_log_filename.c_str());
	}
	_fd = ::open(_fileName.c_str(), _flags, _mode);
}

void RollingFileAppender::doAppend(severity_level level, const char *file, int line, const std::string& message){
	FileAppender::doAppend(level, file, line, message);
	off_t offset = ::lseek(_fd, 0, SEEK_END);
	if (offset < 0) {
		// XXX we got an error, ignore for now
	}
	else {
		if (static_cast<size_t>(offset) >= _maxFileSize) {
			rollOver();
		}
	}
}

std::auto_ptr<Appender> create_roll_file_appender(const FactoryParams& params)
{
	std::string name, filename;
	bool append = true;
	mode_t mode = 664;
	int max_file_size = 0, max_backup_index = 0;
	params.get_for("roll file appender").required("name", name)("filename", filename)("max_file_size", max_file_size)
		("max_backup_index", max_backup_index)
		.optional("append", append)("mode", mode);

	return std::auto_ptr<Appender>(new RollingFileAppender(name, filename, max_file_size, max_backup_index, append, mode));
}

