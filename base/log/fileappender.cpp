#include <memory>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include "fileappender.h"
#include "factoryparams.h"
#include  "zx_utils.h"
#ifdef _WIN32
#include <io.h>
#endif

#pragma warning(disable:4996)

FileAppender::FileAppender(const std::string& name,
	const std::string& fileName,
	bool append,
	mode_t mode) :
	Appender(name),
	_fileName(fileName),
	_flags(O_CREAT | O_APPEND | O_WRONLY),
	_mode(mode) {
	if (!append)
		_flags |= O_TRUNC;
	_fd = ::open(_fileName.c_str(), _flags, _mode);
	if (_fd <= 0) {
		std::ostringstream message_stream;
		message_stream << "file open error, filename=" << _fileName.c_str() << ", error=";
#ifdef _WIN32
		message_stream	<< GetLastError() << "\n";
#else
		message_stream	<< errno << "\n";
#endif
		printf(message_stream.str().c_str());
	}
}

FileAppender::FileAppender(const std::string& name, int fd) :
	Appender(name),
	_fileName(""),
	_fd(fd),
	_flags(O_CREAT | O_APPEND | O_WRONLY),
	_mode(00666) {
}

FileAppender::~FileAppender() {
	close();
}

void FileAppender::close() {
	if (_fd != -1) {
		::close(_fd);
		_fd = -1;
	}
}

void FileAppender::setAppend(bool append) {
	if (append) {
		_flags &= ~O_TRUNC;
	}
	else {
		_flags |= O_TRUNC;
	}
}

bool FileAppender::getAppend() const {
	return (_flags & O_TRUNC) == 0;
}

void FileAppender::setMode(mode_t mode) {
	_mode = mode;
}

mode_t FileAppender::getMode() const {
	return _mode;
}


void FileAppender::doAppend(severity_level level, const char *file, int line, const std::string& message) {
	/*对日志数据进行处理*/
	std::ostringstream message_stream;

	std::string time_RFC3339;
	time_RFC3339 = ZxBase::now2str();
	const std::string& priorityName = getPriorityName((int)level);
	message_stream << "[" << time_RFC3339 << "] ";
	if (file) {
		message_stream << "[" << file << ":" << line  << "] ";
	}
	message_stream << priorityName << " " << message << std::endl;
	
	std::string msg(message_stream.str());
	if (!::write(_fd, msg.data(), msg.length())) {
		// XXX help! help!
	}
}

bool FileAppender::reopen() {
	if (_fileName != "") {
		int fd = ::open(_fileName.c_str(), _flags, _mode);
		if (fd < 0)
			return false;
		else {
			if (_fd != -1)
				::close(_fd);
			_fd = fd;
			return true;
		}
	}
	else {
		return true;
	}
}

std::auto_ptr<Appender> create_file_appender(const FactoryParams& params)
{
	std::string name, filename;
	bool append = true;
	mode_t mode = 666;

	params.get_for("file appender").required("name", name)("filename", filename)
		.optional("append", append)("mode", mode);

	return std::auto_ptr<Appender>(new FileAppender(name, filename, append, mode));
}

