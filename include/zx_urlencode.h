#ifndef _ZX_URLENCODE_H_
#define _ZX_URLENCODE_H_

#include "zx_base.h"
using namespace std;

namespace ZxHttp {

	class BASE_CLASS UrlEncode
	{
	public:
		static std::string urlencode(std::string& text);
		static std::string gethex(unsigned int c);
		static std::string urlencode2(std::string str);
		static std::string urldecode(std::string& strOrg);
	};

}
#endif