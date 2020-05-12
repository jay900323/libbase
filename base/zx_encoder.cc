#include "zx_encoder.h"
#include "zx_base.h"
#include <string.h>

#ifdef _WIN32

#else
#include <iconv.h>
#include <errno.h>
#include <string.h>
#endif

void ZxEncoder::gbk2utf8(char *sOut, int &iMaxOutLen, const char *sIn, int iInLen)
{
#ifdef _WIN32

	wchar_t		wide_string_static[Z_KIBIBYTE], *wide_string = NULL;
	int		wide_size;
	char		*utf8_string = NULL;
	int		utf8_size;
	unsigned int	codepage;

	if (iInLen > iMaxOutLen)
	{
		throw Encoder_Exception("[ZX_Encoder::gbk2utf8] iInLen > iMaxOutLen", 0);
	}

	wide_size = ::MultiByteToWideChar(CP_ACP, 0, sIn, (int)iInLen, NULL, 0);

	if (wide_size > Z_KIBIBYTE)
		wide_string = (wchar_t *)malloc((size_t)wide_size * sizeof(wchar_t));
	else
		wide_string = wide_string_static;

	/* convert from in to wide_string */
	::MultiByteToWideChar(CP_ACP, 0, sIn, (int)iInLen, wide_string, wide_size);

	utf8_size = WideCharToMultiByte(CP_UTF8, 0, wide_string, wide_size, NULL, 0, NULL, NULL);
	//utf8_string = (char *)malloc((size_t)utf8_size + 1/* '\0' */);

	if (utf8_size + 1 > iMaxOutLen) {
		throw Encoder_Exception("[ZX_Encoder::gbk2utf8] utf8_size > iMaxOutLen", 0);
	}

	/* convert from wide_string to utf8_string */
	WideCharToMultiByte(CP_UTF8, 0, wide_string, wide_size, sOut, utf8_size, NULL, NULL);
	sOut[utf8_size] = '\0';

	if (wide_string != wide_string_static)
		free(wide_string);

#else
	char * pIn = (char*)sIn;
	char * pEnd = pIn + iInLen;
	char * pOut = sOut;
	size_t iLeftLen;
	size_t iGbkLen;
	iconv_t cd;

	if (iInLen > iMaxOutLen)
	{
		throw Encoder_Exception("[TC_Encoder::gbk2utf8] iInLen > iMaxOutLen error : ", errno);
	}

	cd = iconv_open("UTF-8", "GBK");
	if (cd == (iconv_t)-1)
	{
		throw Encoder_Exception("[TC_Encoder::gbk2utf8] iconv_open error : ", errno);
	}

	iLeftLen = iMaxOutLen;
	while (pIn < pEnd)
	{
		if ((unsigned char)(*pIn) == 0x80)
		{
			//注意GBK的0x80转换为UTF-8时为E2 82 AC
			*pOut = 0xe2; pOut++; iLeftLen--;
			*pOut = 0x82; pOut++; iLeftLen--;
			*pOut = 0xac; pOut++; iLeftLen--;
			pIn++;
		}
		else if ((unsigned char)(*pIn)<0x80)
		{
			//单字节(GBK: 0x00-0x7F)
			*pOut = *pIn;
			pIn++; pOut++; iLeftLen--;
		}
		else
		{
			//双字节
			iGbkLen = 2;
			int iRet = iconv(cd, &pIn, (size_t *)&iGbkLen, (char **)&pOut, (size_t *)&iLeftLen);
			if (iRet < 0)
			{
				*pOut = ' '; //转换不了替换为空格
				pIn += 2; pOut++; iLeftLen--;
			}
		}
	}

	iconv_close(cd);
	sOut[iMaxOutLen - iLeftLen] = '\0';
	iMaxOutLen = iMaxOutLen - iLeftLen;
#endif
}

string ZxEncoder::gbk2utf8(const string &sIn)
{
#ifdef _WIN32
	const char *in = sIn.data();
	int in_size = sIn.length();

	wchar_t		wide_string_static[Z_KIBIBYTE], *wide_string = NULL;
	int		wide_size;
	char		*utf8_string = NULL;
	int		utf8_size;

	wide_size = ::MultiByteToWideChar(CP_ACP, 0, in, (int)in_size, NULL, 0);

	if (wide_size > Z_KIBIBYTE)
		wide_string = (wchar_t *)malloc((size_t)wide_size * sizeof(wchar_t));
	else
		wide_string = wide_string_static;

	/* convert from in to wide_string */
	::MultiByteToWideChar(CP_ACP, 0, in, (int)in_size, wide_string, wide_size);


	utf8_size = ::WideCharToMultiByte(CP_UTF8, 0, wide_string, wide_size, NULL, 0, NULL, NULL);
	utf8_string = (char *)malloc((size_t)utf8_size + 1);

	/* convert from wide_string to utf8_string */
	::WideCharToMultiByte(CP_UTF8, 0, wide_string, wide_size, utf8_string, utf8_size, NULL, NULL);
	utf8_string[utf8_size] = '\0';

	if (wide_string != wide_string_static && wide_string != (wchar_t *)in)
		free(wide_string);

	std::string sOut;
	sOut = utf8_string;

	if (utf8_string)
		free(utf8_string);

	return sOut;
#else
	iconv_t cd;

	cd = iconv_open("UTF-8", "GBK");
	if (cd == (iconv_t)-1)
	{
		throw Encoder_Exception("[TC_Encoder::gbk2utf8] iconv_open error", errno);
	}

	string sOut;

	for (string::size_type pos = 0; pos < sIn.length(); ++pos)
	{
		if ((unsigned char)sIn[pos] == 0x80)
		{
			//注意GBK的0x80转换为UTF-8时为E2 82 AC
			sOut += 0xe2;
			sOut += 0x82;
			sOut += 0xac;
		}
		else if ((unsigned char)sIn[pos] < 0x80)
		{
			//单字节(GBK: 0x00-0x7F)
			sOut += sIn[pos];
		}
		else
		{
			//双字节
			size_t sizeGbkLen = 2;
			char pIn[128] = "\0";

			strncpy(pIn, sIn.c_str() + pos, sizeGbkLen);
			char *p = pIn;

			size_t sizeLeftLen = 128;
			char pOut[128] = "\0";
			char *o = pOut;
			int iRet = iconv(cd, &p, &sizeGbkLen, (char **)&o, &sizeLeftLen);
			if (iRet < 0)
			{
				//转换不了, 暂时替换为空格
				sOut += ' ';
			}
			else
			{
				sOut += pOut;
			}

			++pos;

		}
	}

	iconv_close(cd);
	return sOut;
#endif
}

void ZxEncoder::utf82gbk(char *sOut, int &iMaxOutLen, const char *sIn, int iInLen)
{
#ifdef _WIN32
	int wide_size = ::MultiByteToWideChar(CP_UTF8, 0, sIn, -1, NULL, 0);
	wchar_t* wide_string = new wchar_t[wide_size + 1];
	::memset(wide_string, 0, wide_size * 2 + 2);
	::MultiByteToWideChar(CP_UTF8, 0, sIn, -1, wide_string, wide_size);

	int gbk_size = ::WideCharToMultiByte(CP_ACP, 0, wide_string, -1, NULL, 0, NULL, NULL);

	if (iMaxOutLen < gbk_size + 1) {
		throw Encoder_Exception("[ZX_Encoder::utf82gbk] iInLen > iMaxOutLen", 0);
	}

	::WideCharToMultiByte(CP_ACP, 0, wide_string, -1, sOut, gbk_size, NULL, NULL);
	sOut[gbk_size] = '\0';

	if (wide_string) delete[] wide_string;

#else
	iconv_t cd;

	cd = iconv_open("GBK", "UTF-8");
	if (cd == (iconv_t)-1)
	{
		throw Encoder_Exception("[TC_Encoder::utf82gbk] iconv_open error", errno);
	}

	char * pIn = (char*)sIn;
	size_t sizeLeftLen = iMaxOutLen;
	size_t sizeInLen = iInLen;
	char* pOut = sOut;

	size_t ret = iconv(cd, &pIn, &sizeInLen, (char **)&sOut, &sizeLeftLen);
	if (ret == (size_t)-1)
	{
		iMaxOutLen = 0;
		iconv_close(cd);
		throw Encoder_Exception("[TC_Encoder::utf82gbk] iconv error", errno);
		return;
	}

	iconv_close(cd);

	pOut[iMaxOutLen - (int)sizeLeftLen] = '\0';

	iMaxOutLen = iMaxOutLen - (int)sizeLeftLen;
#endif
}

string ZxEncoder::utf82gbk(const string &sIn)
{
	if (sIn.length() == 0)
	{
		return "";
	}

	string sOut;

	int iLen = sIn.length() * 2 + 1;
	char *pOut = new char[iLen];

	try
	{
		utf82gbk(pOut, iLen, sIn.c_str(), sIn.length());
	}
	catch (Encoder_Exception& e)
	{
		delete[] pOut;

		throw e;
	}

	sOut.assign(pOut, iLen);

	delete[] pOut;

	return sOut;

}

/**
* \n -> \r\0
* \r -> \r\r
*/
string ZxEncoder::transTo(const string& str, char f /*='\n'*/, char t /*= '\r'*/, char u /*= '\0'*/)
{
	string ret = str;

	for (size_t i = 0; i < ret.length(); ++i)
	{
		if (ret[i] == f)
		{
			ret[i] = t;

			ret.insert(++i, 1, u);
		}
		else if (ret[i] == t)
		{
			ret.insert(++i, 1, t);
		}
	}
	return ret;
}

/**
* \r\0 -> \n
* \r\r -> \r
*/
string ZxEncoder::transFrom(const string& str, char f /*= '\n'*/, char t /*= '\r'*/, char u /*= '\0'*/)
{
	string ret = "";

	for (string::const_iterator it = str.begin()
		; it != str.end()
		; ++it)
	{
		ret.append(1, *it);

		if (*it == t)
		{
			if (*(++it) == u)
			{
				*ret.rbegin() = f;
			}
		}
	}
	return ret;
}