#include "zx_urlencode.h"

namespace ZxHttp {

	const char * hexenc[] = {
		"%00", "%01", "%02", "%03", "%04", "%05", "%06", "%07",
		"%08", "%09", "%0a", "%0b", "%0c", "%0d", "%0e", "%0f",
		"%10", "%11", "%12", "%13", "%14", "%15", "%16", "%17",
		"%18", "%19", "%1a", "%1b", "%1c", "%1d", "%1e", "%1f",
		"%20", "%21", "%22", "%23", "%24", "%25", "%26", "%27",
		"%28", "%29", "%2a", "%2b", "%2c", "%2d", "%2e", "%2f",
		"%30", "%31", "%32", "%33", "%34", "%35", "%36", "%37",
		"%38", "%39", "%3a", "%3b", "%3c", "%3d", "%3e", "%3f",
		"%40", "%41", "%42", "%43", "%44", "%45", "%46", "%47",
		"%48", "%49", "%4a", "%4b", "%4c", "%4d", "%4e", "%4f",
		"%50", "%51", "%52", "%53", "%54", "%55", "%56", "%57",
		"%58", "%59", "%5a", "%5b", "%5c", "%5d", "%5e", "%5f",
		"%60", "%61", "%62", "%63", "%64", "%65", "%66", "%67",
		"%68", "%69", "%6a", "%6b", "%6c", "%6d", "%6e", "%6f",
		"%70", "%71", "%72", "%73", "%74", "%75", "%76", "%77",
		"%78", "%79", "%7a", "%7b", "%7c", "%7d", "%7e", "%7f",
		"%80", "%81", "%82", "%83", "%84", "%85", "%86", "%87",
		"%88", "%89", "%8a", "%8b", "%8c", "%8d", "%8e", "%8f",
		"%90", "%91", "%92", "%93", "%94", "%95", "%96", "%97",
		"%98", "%99", "%9a", "%9b", "%9c", "%9d", "%9e", "%9f",
		"%a0", "%a1", "%a2", "%a3", "%a4", "%a5", "%a6", "%a7",
		"%a8", "%a9", "%aa", "%ab", "%ac", "%ad", "%ae", "%af",
		"%b0", "%b1", "%b2", "%b3", "%b4", "%b5", "%b6", "%b7",
		"%b8", "%b9", "%ba", "%bb", "%bc", "%bd", "%be", "%bf",
		"%c0", "%c1", "%c2", "%c3", "%c4", "%c5", "%c6", "%c7",
		"%c8", "%c9", "%ca", "%cb", "%cc", "%cd", "%ce", "%cf",
		"%d0", "%d1", "%d2", "%d3", "%d4", "%d5", "%d6", "%d7",
		"%d8", "%d9", "%da", "%db", "%dc", "%dd", "%de", "%df",
		"%e0", "%e1", "%e2", "%e3", "%e4", "%e5", "%e6", "%e7",
		"%e8", "%e9", "%ea", "%eb", "%ec", "%ed", "%ee", "%ef",
		"%f0", "%f1", "%f2", "%f3", "%f4", "%f5", "%f6", "%f7",
		"%f8", "%f9", "%fa", "%fb", "%fc", "%fd", "%fe", "%ff"
	};

	std::string UrlEncode::urlencode(std::string& text)
	{
		size_t len = text.length();
		std::string encoded = "";
		for (size_t i = 0; i < len; i++)
		{
			unsigned char wch = text.at(i);
			if ('A' <= wch && wch <= 'Z') {
				encoded += wch;
			}
			else if ('a' <= wch && wch <= 'z') {
				encoded += wch;
			}
			else if ('0' <= wch && wch <= '9') {
				encoded += wch;
			}
			else if (wch == ' ') {
				encoded += '+';
			}
			else if (wch == '('
				|| wch == ')') {
				encoded += wch;
			}
			else if (wch == '-' || wch == '_'
				|| wch == '.' || wch == '!'
				|| wch == '~' || wch == '*'
				|| wch == '\'') {
				encoded += hexenc[wch];
			}
			else if (wch <= 0x007f) {        // other ASCII
				encoded += hexenc[wch];


			}
			else if (wch <= 0x07FF) {        // non-ASCII <= 0x7FF
				encoded += hexenc[0xc0 | (wch >> 6)];
				encoded += hexenc[0x80 | (wch & 0x3F)];
			}
			else {                    // 0x7FF < ch <= 0xFFFF
				encoded += hexenc[0xe0 | (wch >> 12)];
				encoded += hexenc[0x80 | ((wch >> 6) & 0x3F)];
				encoded += hexenc[0x80 | (wch & 0x3F)];
			}
		}
		return encoded.c_str();//ref new String(encoded.c_str());    
	}


	std::string UrlEncode::gethex(unsigned int c)//参数必须是int，你懂的
	{
		//std::ostringstream stm ;
		//stm << '%' << std::hex << std::nouppercase << c ;
		char buf[20] = { 0 };
		sprintf(buf, "%%%02X", c);
		return buf;
	}

	std::string UrlEncode::urlencode2(std::string str)
	{
		//不编码字符
		static const std::string unreserved = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz"
			"-_.~";

		std::string r;

		for (unsigned int i = 0; i < str.length(); i += 1)
		{
			unsigned char c = str.at(i);//这里必须是无符号型
			if (unreserved.find(c) != -1) r += c; else r += UrlEncode::gethex(c);
		}

		return r;
	}

	char fromHex(const char &x)
	{
		return isdigit(x) ? x - '0' : x - 'A' + 10;
	}

	std::string UrlEncode::urldecode(std::string& strOrg)
	{
		std::string sOut;
		for (size_t ix = 0; ix < strOrg.size(); ix++)
		{
			char ch = 0;
			if (strOrg[ix] == '%')
			{
				ch = (fromHex(strOrg[ix + 1]) << 4);
				ch |= fromHex(strOrg[ix + 2]);
				ix += 2;
			}
			else if (strOrg[ix] == '+')
			{
				ch = ' ';
			}
			else
			{
				ch = strOrg[ix];
			}
			sOut += (char)ch;
		}
		return sOut;
	}

}