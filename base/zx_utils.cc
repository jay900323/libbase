#include "zx_utils.h"
#include <signal.h>
#include <string.h>
#include <cmath>
#include <sstream>
#ifndef _WIN32
#include <sys/time.h>
#include <fcntl.h>
#endif

template <>
string ZxBase::tostr<bool>(const bool &t)
{
	char buf[2];
	buf[0] = t ? '1' : '0';
	buf[1] = '\0';
	return string(buf);
}


template <>
string ZxBase::tostr<char>(const char &t)
{
	char buf[2];
	snprintf(buf, 2, "%c", t);
	return string(buf);
}

template <>
string ZxBase::tostr<unsigned char>(const unsigned char &t)
{
	char buf[2];
	snprintf(buf, 2, "%c", t);
	return string(buf);
}

template <>
string ZxBase::tostr<short>(const short &t)
{
	char buf[16];
	snprintf(buf, 16, "%d", t);
	return string(buf);
}

template <>
string ZxBase::tostr<unsigned short>(const unsigned short &t)
{
	char buf[16];
	snprintf(buf, 16, "%u", t);
	return string(buf);
}

template <>
string ZxBase::tostr<int>(const int &t)
{
	char buf[16];
	snprintf(buf, 16, "%d", t);
	return string(buf);
}

template <>
string ZxBase::tostr<unsigned int>(const unsigned int &t)
{
	char buf[16];
	snprintf(buf, 16, "%u", t);
	return string(buf);
}

template <>
string ZxBase::tostr<long>(const long &t)
{
	char buf[32];
	snprintf(buf, 32, "%ld", t);
	return string(buf);
}

template <>
string ZxBase::tostr<long long>(const long long &t)
{
	char buf[32];
	snprintf(buf, 32, "%lld", t);
	return string(buf);
}


template <>
string ZxBase::tostr<unsigned long>(const unsigned long &t)
{
	char buf[32];
	snprintf(buf, 32, "%lu", t);
	return string(buf);
}

template <>
string ZxBase::tostr<float>(const float &t)
{
	char buf[32];
	snprintf(buf, 32, "%.5f", t);
	string s(buf);

	//去掉无效0, eg. 1.0300 -> 1.03;1.00 -> 1
	bool bFlag = false;
	int pos = int(s.size() - 1);
	for (; pos > 0; --pos)
	{
		if (s[pos] == '0')
		{
			bFlag = true;
			if (s[pos - 1] == '.')
			{
				//-2为了去掉"."号
				pos -= 2;
				break;
			}
		}
		else
		{
			break;
		}
	}

	if (bFlag)
		s = s.substr(0, pos + 1);

	return s;
}

template <>
string ZxBase::tostr<double>(const double &t)
{
	char buf[32];
	snprintf(buf, 32, "%.5f", t);
	string s(buf);

	//去掉无效0, eg. 1.0300 -> 1.03;1.00 -> 1
	bool bFlag = false;
	int pos = int(s.size() - 1);
	for (; pos > 0; --pos)
	{
		if (s[pos] == '0')
		{
			bFlag = true;
			if (s[pos - 1] == '.')
			{
				//-2为了去掉"."号
				pos -= 2;
				break;
			}
		}
		else
		{
			break;
		}
	}

	if (bFlag)
		s = s.substr(0, pos + 1);

	return s;

}

template <>
string ZxBase::tostr<long double>(const long double &t)
{
	char buf[32];
	snprintf(buf, 32, "%Lf", t);
	string s(buf);

	//去掉无效0, eg. 1.0300 -> 1.03;1.00 -> 1
	bool bFlag = false;
	int pos = int(s.size() - 1);
	for (; pos > 0; --pos)
	{
		if (s[pos] == '0')
		{
			bFlag = true;
			if (s[pos - 1] == '.')
			{
				//-2为了去掉"."号
				pos -= 2;
				break;
			}
		}
		else
		{
			break;
		}
	}

	if (bFlag)
		s = s.substr(0, pos + 1);

	return s;

}

template <>
string ZxBase::tostr<std::string>(const std::string &t)
{
	return t;
}

string ZxBase::trim(const string &sStr, const string &s, bool bChar)
{
	if (sStr.empty())
	{
		return sStr;
	}

	/**
	* 将完全与s相同的字符串去掉
	*/
	if (!bChar)
	{
		return trimright(trimleft(sStr, s, false), s, false);
	}

	return trimright(trimleft(sStr, s, true), s, true);
}

string ZxBase::trimleft(const string &sStr, const string &s, bool bChar)
{
	if (sStr.empty())
	{
		return sStr;
	}

	/**
	* 去掉sStr左边的字符串s
	*/
	if (!bChar)
	{
		if (sStr.length() < s.length())
		{
			return sStr;
		}

		if (sStr.compare(0, s.length(), s) == 0)
		{
			return sStr.substr(s.length());
		}

		return sStr;
	}

	/**
	* 去掉sStr左边的 字符串s中的字符
	*/
	string::size_type pos = 0;
	while (pos < sStr.length())
	{
		if (s.find_first_of(sStr[pos]) == string::npos)
		{
			break;
		}

		pos++;
	}

	if (pos == 0) return sStr;

	return sStr.substr(pos);
}

string ZxBase::trimright(const string &sStr, const string &s, bool bChar)
{
	if (sStr.empty())
	{
		return sStr;
	}

	/**
	* 去掉sStr右边的字符串s
	*/
	if (!bChar)
	{
		if (sStr.length() < s.length())
		{
			return sStr;
		}

		if (sStr.compare(sStr.length() - s.length(), s.length(), s) == 0)
		{
			return sStr.substr(0, sStr.length() - s.length());
		}

		return sStr;
	}

	/**
	* 去掉sStr右边的 字符串s中的字符
	*/
	string::size_type pos = sStr.length();
	while (pos != 0)
	{
		if (s.find_first_of(sStr[pos - 1]) == string::npos)
		{
			break;
		}

		pos--;
	}

	if (pos == sStr.length()) return sStr;

	return sStr.substr(0, pos);
}

string ZxBase::lower(const string &s)
{
	string sString = s;
	for (string::iterator iter = sString.begin(); iter != sString.end(); ++iter)
	{
		*iter = tolower(*iter);
	}

	return sString;
}

string ZxBase::upper(const string &s)
{
	string sString = s;

	for (string::iterator iter = sString.begin(); iter != sString.end(); ++iter)
	{
		*iter = toupper(*iter);
	}

	return sString;
}

bool ZxBase::isdigit(const string &sInput)
{
	string::const_iterator iter = sInput.begin();

	if (sInput.empty())
	{
		return false;
	}

	while (iter != sInput.end())
	{
		if (!::isdigit(*iter))
		{
			return false;
		}
		++iter;
	}
	return true;
}

int ZxBase::str2tm(const string &sString, const string &sFormat, struct tm &stTm)
{
#ifdef _WIN32
	int year, month, day, hour, minute, second;
	if (sscanf(sString.data(), sFormat.data(), &year, &month, &day, &hour, &minute, &second) == 6) {
		stTm.tm_year = year - 1900;
		stTm.tm_mon = month - 1;
		stTm.tm_mday = day;
		stTm.tm_hour = hour;
		stTm.tm_min = minute;
		stTm.tm_sec = second;
		stTm.tm_isdst = 0;
		return 0;
	}
	else {
		return -1;
	}

#else
	char *p = strptime(sString.c_str(), sFormat.c_str(), &stTm);
	return (p != NULL) ? 0 : -1;
#endif

}

int ZxBase::strgmt2tm(const string &sString, struct tm &stTm)
{
	return str2tm(sString, "%a, %d %b %Y %H:%M:%S GMT", stTm);
}

string ZxBase::tm2str(const struct tm &stTm, const string &sFormat)
{
	char sTimeString[255] = "\0";

	strftime(sTimeString, sizeof(sTimeString), sFormat.c_str(), &stTm);

	return string(sTimeString);
}

string ZxBase::tm2str(const time_t &t, const string &sFormat)
{
	struct tm tt;
#ifdef _WIN32
	tt = *(localtime(&t));
#else
	localtime_r(&t, &tt);
#endif
	return tm2str(tt, sFormat);
}

string ZxBase::now2str(const string &sFormat)
{
	time_t t = time(NULL);
	return tm2str(t, sFormat.c_str());
}

string ZxBase::now2GMTstr()
{
	time_t t = time(NULL);
	return tm2GMTstr(t);
}

string ZxBase::tm2GMTstr(const time_t &t)
{
	struct tm tt;
#ifdef _WIN32
	tt = *gmtime(&t);
#else
	gmtime_r(&t, &tt);
#endif
	return tm2str(tt, "%a, %d %b %Y %H:%M:%S GMT");

}

string ZxBase::tm2GMTstr(const struct tm &stTm)
{
	return tm2str(stTm, "%a, %d %b %Y %H:%M:%S GMT");
}

string ZxBase::nowdate2str()
{
	return now2str("%Y%m%d");
}

string ZxBase::nowtime2str()
{
	return now2str("%H%M%S");
}
#ifndef _WIN32
int64_t ZxBase::now2ms()
{
	struct timeval tv;

	gettimeofday(&tv, 0);

	return tv.tv_sec * (int64_t)1000 + tv.tv_usec / 1000;
}
#endif

#ifndef _WIN32
int64_t ZxBase::now2us()
{
	struct timeval tv;

	gettimeofday(&tv, 0);

	return tv.tv_sec * (int64_t)1000000 + tv.tv_usec;
}
#endif

//参照phorix的优化
static char c_b2s[256][4] = { "00","01","02","03","04","05","06","07","08","09","0a","0b","0c","0d","0e","0f","10","11","12","13","14","15","16","17","18","19","1a","1b","1c","1d","1e","1f","20","21","22","23","24","25","26","27","28","29","2a","2b","2c","2d","2e","2f","30","31","32","33","34","35","36","37","38","39","3a","3b","3c","3d","3e","3f","40","41","42","43","44","45","46","47","48","49","4a","4b","4c","4d","4e","4f","50","51","52","53","54","55","56","57","58","59","5a","5b","5c","5d","5e","5f","60","61","62","63","64","65","66","67","68","69","6a","6b","6c","6d","6e","6f","70","71","72","73","74","75","76","77","78","79","7a","7b","7c","7d","7e","7f","80","81","82","83","84","85","86","87","88","89","8a","8b","8c","8d","8e","8f","90","91","92","93","94","95","96","97","98","99","9a","9b","9c","9d","9e","9f","a0","a1","a2","a3","a4","a5","a6","a7","a8","a9","aa","ab","ac","ad","ae","af","b0","b1","b2","b3","b4","b5","b6","b7","b8","b9","ba","bb","bc","bd","be","bf","c0","c1","c2","c3","c4","c5","c6","c7","c8","c9","ca","cb","cc","cd","ce","cf","d0","d1","d2","d3","d4","d5","d6","d7","d8","d9","da","db","dc","dd","de","df","e0","e1","e2","e3","e4","e5","e6","e7","e8","e9","ea","eb","ec","ed","ee","ef","f0","f1","f2","f3","f4","f5","f6","f7","f8","f9","fa","fb","fc","fd","fe","ff" };

string ZxBase::bin2str(const void *buf, size_t len, const string &sSep, size_t lines)
{
	if (buf == NULL || len <= 0)
	{
		return "";
	}

	string sOut;
	const unsigned char *p = (const unsigned char *)buf;

	for (size_t i = 0; i < len; ++i, ++p)
	{
		sOut += c_b2s[*p][0];
		sOut += c_b2s[*p][1];
		sOut += sSep;

		//换行
		if ((lines != 0) && ((i + 1) % lines == 0))
		{
			sOut += "\n";
		}
	}

	return sOut;
}

string ZxBase::bin2str(const string &sBinData, const string &sSep, size_t lines)
{
	return bin2str((const void *)sBinData.data(), sBinData.length(), sSep, lines);
}

int ZxBase::str2bin(const char *psAsciiData, unsigned char *sBinData, int iBinSize)
{
	int iAsciiLength = strlen(psAsciiData);

	int iRealLength = (iAsciiLength / 2 > iBinSize) ? iBinSize : (iAsciiLength / 2);
	for (int i = 0; i < iRealLength; i++)
	{
		sBinData[i] = x2c(psAsciiData + i * 2);
	}
	return iRealLength;
}

string ZxBase::str2bin(const string &sString, const string &sSep, size_t lines)
{
	const char *psAsciiData = sString.c_str();

	int iAsciiLength = sString.length();
	string sBinData;
	for (int i = 0; i < iAsciiLength; i++)
	{
		sBinData += x2c(psAsciiData + i);
		i++;
		i += sSep.length(); //过滤掉分隔符

		if (lines != 0 && sBinData.length() % lines == 0)
		{
			i++;    //过滤掉回车
		}
	}

	return sBinData;
}

char ZxBase::x2c(const string &sWhat)
{
	register char digit;

	if (sWhat.length() < 2)
	{
		return '\0';
	}

	digit = (sWhat[0] >= 'A' ? ((sWhat[0] & 0xdf) - 'A') + 10 : (sWhat[0] - '0'));
	digit *= 16;
	digit += (sWhat[1] >= 'A' ? ((sWhat[1] & 0xdf) - 'A') + 10 : (sWhat[1] - '0'));

	return(digit);
}

string ZxBase::replace(const string &sString, const string &sSrc, const string &sDest)
{
	if (sSrc.empty())
	{
		return sString;
	}

	string sBuf = sString;

	string::size_type pos = 0;

	while ((pos = sBuf.find(sSrc, pos)) != string::npos)
	{
		sBuf.replace(pos, sSrc.length(), sDest);
		pos += sDest.length();
	}

	return sBuf;
}

string ZxBase::replace(const string &sString, const map<string, string>& mSrcDest)
{
	if (sString.empty())
	{
		return sString;
	}

	string tmp = sString;
	map<string, string>::const_iterator it = mSrcDest.begin();

	while (it != mSrcDest.end())
	{

		string::size_type pos = 0;
		while ((pos = tmp.find(it->first, pos)) != string::npos)
		{
			tmp.replace(pos, it->first.length(), it->second);
			pos += it->second.length();
		}

		++it;
	}

	return tmp;
}

bool ZxBase::matchPeriod(const string& s, const string& pat)
{
	if (s.empty())
	{
		return false;
	}

	if (pat.empty())
	{
		return true;
	}

	if (pat.find('*') == string::npos)
	{
		return s == pat;
	}

	string::size_type sIndex = 0;
	string::size_type patIndex = 0;
	do
	{
		if (pat[patIndex] == '*')
		{
			if (s[sIndex] == '.')
			{
				return false;
			}

			while (sIndex < s.size() && s[sIndex] != '.')
			{
				++sIndex;
			}
			patIndex++;
		}
		else
		{
			if (pat[patIndex] != s[sIndex])
			{
				return false;
			}
			++sIndex;
			++patIndex;
		}
	} while (sIndex < s.size() && patIndex < pat.size());

	return sIndex == s.size() && patIndex == pat.size();
}

bool ZxBase::matchPeriod(const string& s, const vector<string>& pat)
{
	for (size_t i = 0; i < pat.size(); i++)
	{
		if (ZxBase::matchPeriod(s, pat[i]))
		{
			return true;
		}
	}
	return false;
}

#ifndef _WIN32
void ZxBase::daemon()
{
	pid_t pid;
	int fd = 0;
	
	if ((pid = fork()) != 0)
	{
		exit(0);
	}

	setsid();

	signal(SIGINT, SIG_IGN);
	signal(SIGHUP, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	ignorePipe();

	if ((pid = fork()) != 0)
	{
		//父进程结束,变成daemon
		exit(0);
	}

	/* Dup stdin, stdout and stderr to /dev/null */
	if ((fd = open("/dev/null", O_RDWR)) >= 0) {
		dup2(fd, 0);
		dup2(fd, 1);
		dup2(fd, 2);

		close(fd);
	}

	umask(0);

	//  chdir("/");
}


void ZxBase::ignorePipe()
{
	struct sigaction sig;

	memset(&sig, 0, sizeof(struct sigaction));

	sig.sa_handler = SIG_IGN;
	sig.sa_flags = 0;
	sigemptyset(&sig.sa_mask);
	sigaction(SIGPIPE, &sig, NULL);
}

#endif

bool ZxBase::isPrimeNumber(size_t n)
{
	size_t nn = (size_t)sqrt((double)n);
	for (size_t i = 2; i < nn; i++)
	{
		if (n % i == 0)
		{
			return false;
		}
	}
	return true;
}

size_t ZxBase::toSize(const string &s, size_t iDefaultSize)
{
	if (s.empty())
	{
		return iDefaultSize;
	}

	char c = s[s.length() - 1];
	if (c != 'K' && c != 'M' && c != 'G' && ZxBase::trim(s) == ZxBase::tostr(atoi(s.c_str())))
	{
		//没有后缀, 且转换是正确的
		return (size_t)(atoi(s.c_str()));
	}
	else if (c == 'K' || c == 'M' || c == 'G')
	{
		if (s.length() == 1)
		{
			return iDefaultSize;
		}
		
		float n = atof(s.substr(0, s.length() - 1).c_str());

		if (ZxBase::trim(s.substr(0, s.length() - 1)) != ZxBase::tostr(n))
		{
			return iDefaultSize;
		}

		if (c == 'K')
		{
			return (size_t)(n * 1024);
		}
		if (c == 'M')
		{
			return (size_t)(n * 1024 * 1024);
		}
		if (c == 'G')
		{
			return (size_t)(n * 1024 * 1024 * 1024);
		}
	}

	return iDefaultSize;
}

void ZxBase::strsplit(const std::string& s, std::vector<std::string>& v, const std::string& sep)
{
	std::string::size_type pos1, pos2;
	pos2 = s.find(sep);
	pos1 = 0;
	while (std::string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + sep.size();
		pos2 = s.find(sep, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}

bool ZxBase::start_with(const std::string &str, const std::string &prefix)
{
	size_t len1 = str.length();
	size_t len2 = prefix.length();
	if (len1 < len2)
	{
		return false;
	}
	return (!str.compare(0, len2, prefix));
}

bool ZxBase::end_with(const std::string &str, const std::string &suffix)
{
	size_t len1 = str.length();
	size_t len2 = suffix.length();
	if (len1 < len2)
	{
		return false;
	}
	return (!str.compare(len1 - len2, len2, suffix));
}

template<typename T>
string ZxBase::tostr(const T &t)
{
	ostringstream sBuffer;
	sBuffer << t;
	return sBuffer.str();
}

template<typename T>
string ZxBase::tostr(const vector<T> &t)
{
	string s;
	for (size_t i = 0; i < t.size(); i++)
	{
		s += tostr(t[i]);
		s += " ";
	}
	return s;
}

template<typename K, typename V, typename D, typename A>
string ZxBase::tostr(const map<K, V, D, A> &t)
{
	string sBuffer;
	typename map<K, V, D, A>::const_iterator it = t.begin();
	while (it != t.end())
	{
		sBuffer += " [";
		sBuffer += tostr(it->first);
		sBuffer += "]=[";
		sBuffer += tostr(it->second);
		sBuffer += "] ";
		++it;
	}
	return sBuffer;
}

template<typename K, typename V, typename D, typename A>
string ZxBase::tostr(const multimap<K, V, D, A> &t)
{
	string sBuffer;
	typename multimap<K, V, D, A>::const_iterator it = t.begin();
	while (it != t.end())
	{
		sBuffer += " [";
		sBuffer += tostr(it->first);
		sBuffer += "]=[";
		sBuffer += tostr(it->second);
		sBuffer += "] ";
		++it;
	}
	return sBuffer;
}

template<typename F, typename S>
string ZxBase::tostr(const pair<F, S> &itPair)
{
	string sBuffer;
	sBuffer += "[";
	sBuffer += tostr(itPair.first);
	sBuffer += "]=[";
	sBuffer += tostr(itPair.second);
	sBuffer += "]";
	return sBuffer;
}

template <typename InputIter>
string ZxBase::tostr(InputIter iFirst, InputIter iLast, const string &sSep)
{
	string sBuffer;
	InputIter it = iFirst;

	while (it != iLast)
	{
		sBuffer += tostr(*it);
		++it;

		if (it != iLast)
		{
			sBuffer += sSep;
		}
		else
		{
			break;
		}
	}

	return sBuffer;
}