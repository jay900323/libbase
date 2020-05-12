#include "zx_except.h"
#include <string.h>
#ifndef _WIN32
#include <execinfo.h>
#endif

Exception::Exception(const std::string &buffer)
	:_buffer(buffer), _code(0)
{
	//    getBacktrace();
}

Exception::Exception(const std::string &buffer, int err)
{
	_buffer = buffer + " :" + strerror(err);
	_code = err;
	//    getBacktrace();
}

Exception::~Exception() throw()
{
}

const char* Exception::what() const throw()
{
	return _buffer.c_str();
}

void Exception::getBacktrace()
{
#ifndef _WIN32
	void * array[64];
	int nSize = backtrace(array, 64);
	char ** symbols = backtrace_symbols(array, nSize);

	for (int i = 0; i < nSize; i++)
	{
		_buffer += symbols[i];
		_buffer += "\n";
	}
	free(symbols);
#endif
}