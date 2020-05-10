#ifndef _ZX_EXCEPTION_H
#define _ZX_EXCEPTION_H

#include <exception>
#include <string>
#include "zx_base.h"

class BASE_CLASS Exception : public ::std::exception
{
public:
	Exception(const std::string &buffer);
	Exception(const std::string &buffer, int err);
	virtual ~Exception() throw();
	virtual const char* what() const throw();
	int getErrCode() { return _code; }
private:
	void getBacktrace();
private:
	std::string  _buffer;
	int     _code;
}; // class Exception

#endif