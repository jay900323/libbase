#ifndef __ZX_UTILS_H
#define __ZX_UTILS_H

#ifndef __USE_XOPEN
#define __USE_XOPEN
#endif

#include "zx_base.h"
#include <vector>
#include <string>
#include <map>
#include <time.h>
using namespace std;


/**
* @brief  基础工具类，提供了一些非常基本的函数使用.
*
* 这些函数都是以静态函数提供。 包括以下几种函数:
*
* Trim类函数,大小写转换函数,分隔字符串函数（直接分隔字符串，
*
* 数字等）,时间相关函数,字符串转换函数,二进制字符串互转函数,
*
* 替换字符串函数,Ip匹配函数,判断一个数是否是素数等
*/
class BASE_CLASS ZxBase
{
public:

	/**
	* @brief  去掉头部以及尾部的字符或字符串.
	*
	* @param sStr    输入字符串
	* @param s       需要去掉的字符
	* @param bChar   如果为true, 则去掉s中每个字符; 如果为false, 则去掉s字符串
	* @return string 返回去掉后的字符串
	*/
	static string trim(const string &sStr, const string &s = " \r\n\t", bool bChar = true);

	/**
	* @brief  去掉左边的字符或字符串.
	*
	* @param sStr    输入字符串
	* @param s       需要去掉的字符
	* @param bChar   如果为true, 则去掉s中每个字符; 如果为false, 则去掉s字符串
	* @return string 返回去掉后的字符串
	*/
	static string trimleft(const string &sStr, const string &s = " \r\n\t", bool bChar = true);

	/**
	* @brief  去掉右边的字符或字符串.
	*
	* @param sStr    输入字符串
	* @param s       需要去掉的字符
	* @param bChar   如果为true, 则去掉s中每个字符; 如果为false, 则去掉s字符串
	* @return string 返回去掉后的字符串
	*/
	static string trimright(const string &sStr, const string &s = " \r\n\t", bool bChar = true);

	/**
	* @brief  字符串转换成小写.
	*
	* @param sString  字符串
	* @return string  转换后的字符串
	*/
	static string lower(const string &sString);

	/**
	* @brief  字符串转换成大写.
	*
	* @param sString  字符串
	* @return string  转换后的大写的字符串
	*/
	static string upper(const string &sString);

	/**
	* @brief  字符串是否都是数字的.
	*
	* @param sString  字符串
	* @return bool    是否是数字
	*/
	static bool isdigit(const string &sInput);

	/**
	* @brief  字符串转换成时间结构.
	*
	* @param sString  字符串时间格式
	* @param sFormat  格式
	* @param stTm     时间结构
	* @return         0: 成功, -1:失败
	*/
	static int str2tm(const string &sString, const string &sFormat, struct tm &stTm);

	/**
	* @brief GMT格式的时间转化为时间结构
	*
	* eg.Sat, 06 Feb 2010 09:29:29 GMT, %a, %d %b %Y %H:%M:%S GMT
	*
	* 可以用mktime换成time_t, 但是注意时区 可以用mktime(&stTm)
	*
	* - timezone换成本地的秒(time(NULL)值相同) timezone是系统的 ,
	*
	* 需要extern long timezone;
	*
	* @param sString  GMT格式的时间
	* @param stTm     转换后的时间结构
	* @return         0: 成功, -1:失败
	*/
	static int strgmt2tm(const string &sString, struct tm &stTm);

	/**
	* @brief  时间转换成字符串.
	*
	* @param stTm     时间结构
	* @param sFormat  需要转换的目标格式，默认为紧凑格式
	* @return string  转换后的时间字符串
	*/
	static string tm2str(const struct tm &stTm, const string &sFormat = "%Y-%m-%d %H:%M:%S");

	/**
	* @brief  时间转换成字符串.
	*
	* @param t        时间结构
	* @param sFormat  需要转换的目标格式，默认为紧凑格式
	* @return string  转换后的时间字符串
	*/
	static string tm2str(const time_t &t, const string &sFormat = "%Y-%m-%d %H:%M:%S");

	/**
	* @brief  当前时间转换成紧凑格式字符串
	* @param sFormat 格式，默认为紧凑格式
	* @return string 转换后的时间字符串
	*/
	static string now2str(const string &sFormat = "%Y-%m-%d %H:%M:%S");

	/**
	* @brief  时间转换成GMT字符串，GMT格式:Fri, 12 Jan 2001 18:18:18 GMT
	* @param stTm    时间结构
	* @return string GMT格式的时间字符串
	*/
	static string tm2GMTstr(const struct tm &stTm);

	/**
	* @brief  时间转换成GMT字符串，GMT格式:Fri, 12 Jan 2001 18:18:18 GMT
	* @param stTm    时间结构
	* @return string GMT格式的时间字符串
	*/
	static string tm2GMTstr(const time_t &t);

	/**
	* @brief  当前时间转换成GMT字符串，GMT格式:Fri, 12 Jan 2001 18:18:18 GMT
	* @return string GMT格式的时间字符串
	*/
	static string now2GMTstr();

	/**
	* @brief  当前的日期(年月日)转换成字符串(%Y%m%d).
	*
	* @return string (%Y%m%d)格式的时间字符串
	*/
	static string nowdate2str();

	/**
	* @brief  当前的时间(时分秒)转换成字符串(%H%M%S).
	*
	* @return string (%H%M%S)格式的时间字符串
	*/
	static string nowtime2str();

	/**
	* @brief  获取当前时间的的毫秒数.
	*
	* @return int64_t 当前时间的的毫秒数
	*/
#ifndef _WIN32
	static int64_t now2ms();
#endif
	/**
	* @brief  取出当前时间的微秒.
	*
	* @return int64_t 取出当前时间的微秒
	*/
#ifndef _WIN32
	static int64_t now2us();
#endif

	/**
	* @brief T型转换成字符串，只要T能够使用ostream对象用<<重载,即可以被该函数支持
	* @param t 要转换的数据
	* @return  转换后的字符串
	*/
	template<typename T>
	static string tostr(const T &t);

	/**
	* @brief  vector转换成string.
	*
	* @param t 要转换的vector型的数据
	* @return  转换后的字符串
	*/
	template<typename T>
	static string tostr(const vector<T> &t);

	/**
	* @brief  把map输出为字符串.
	*
	* @param map<K, V, D, A>  要转换的map对象
	* @return                    string 输出的字符串
	*/
	template<typename K, typename V, typename D, typename A>
	static string tostr(const map<K, V, D, A> &t);

	/**
	* @brief  map输出为字符串.
	*
	* @param multimap<K, V, D, A>  map对象
	* @return                      输出的字符串
	*/
	template<typename K, typename V, typename D, typename A>
	static string tostr(const multimap<K, V, D, A> &t);

	/**
	* @brief  pair 转化为字符串，保证map等关系容器可以直接用tostr来输出
	* @param pair<F, S> pair对象
	* @return           输出的字符串
	*/
	template<typename F, typename S>
	static string tostr(const pair<F, S> &itPair);

	/**
	* @brief  container 转换成字符串.
	*
	* @param iFirst  迭代器
	* @param iLast   迭代器
	* @param sSep    两个元素之间的分隔符
	* @return        转换后的字符串
	*/
	template <typename InputIter>
	static string tostr(InputIter iFirst, InputIter iLast, const string &sSep = "|");

	/**
	* @brief  二进制数据转换成字符串.
	*
	* @param buf     二进制buffer
	* @param len     buffer长度
	* @param sSep    分隔符
	* @param lines   多少个字节换一行, 默认0表示不换行
	* @return        转换后的字符串
	*/
	static string bin2str(const void *buf, size_t len, const string &sSep = "", size_t lines = 0);

	/**
	* @brief  二进制数据转换成字符串.
	*
	* @param sBinData  二进制数据
	* @param sSep     分隔符
	* @param lines    多少个字节换一行, 默认0表示不换行
	* @return         转换后的字符串
	*/
	static string bin2str(const string &sBinData, const string &sSep = "", size_t lines = 0);

	/**
	* @brief   字符串转换成二进制.
	*
	* @param psAsciiData 字符串
	* @param sBinData    二进制数据
	* @param iBinSize    需要转换的字符串长度
	* @return            转换长度，小于等于0则表示失败
	*/
	static int str2bin(const char *psAsciiData, unsigned char *sBinData, int iBinSize);

	/**
	* @brief  字符串转换成二进制.
	*
	* @param sBinData  要转换的字符串
	* @param sSep      分隔符
	* @param lines     多少个字节换一行, 默认0表示不换行
	* @return          转换后的二进制数据
	*/
	static string str2bin(const string &sBinData, const string &sSep = "", size_t lines = 0);

	/**
	* @brief  替换字符串.
	*
	* @param sString  输入字符串
	* @param sSrc     原字符串
	* @param sDest    目的字符串
	* @return string  替换后的字符串
	*/
	static string replace(const string &sString, const string &sSrc, const string &sDest);

	/**
	* @brief  批量替换字符串.
	*
	* @param sString  输入字符串
	* @param mSrcDest  map<原字符串,目的字符串>
	* @return string  替换后的字符串
	*/
	static string replace(const string &sString, const map<string, string>& mSrcDest);

	/**
	* @brief 匹配以.分隔的字符串，pat中*则代表通配符，代表非空的任何字符串
	* s为空, 返回false ，pat为空, 返回true
	* @param s    普通字符串
	* @param pat  带*则被匹配的字符串，用来匹配ip地址
	* @return     是否匹配成功
	*/
	static bool matchPeriod(const string& s, const string& pat);

	/**
	* @brief  匹配以.分隔的字符串.
	*
	* @param s   普通字符串
	* @param pat vector中的每个元素都是带*则被匹配的字符串，用来匹配ip地址
	* @return    是否匹配成功
	*/
	static bool matchPeriod(const string& s, const vector<string>& pat);

	/**
	* @brief  判断一个数是否为素数.
	*
	* @param n  需要被判断的数据
	* @return   true代表是素数，false表示非素数
	*/
	static bool isPrimeNumber(size_t n);

	/**
	* @brief  daemon
	*/
#ifndef _WIN32
	static void daemon();
#endif
	/**
	* @brief  忽略管道异常
	*/
#ifndef _WIN32
	static void ignorePipe();
#endif
	/**
	* @brief  将一个string类型转成一个字节 .
	*
	* @param sWhat 要转换的字符串
	* @return char    转换后的字节
	*/
	static char x2c(const string &sWhat);

	/**
	* @brief  大小字符串换成字节数，支持K, M, G两种 例如: 1K, 3M, 4G, 4.5M, 2.3G
	* @param s            要转换的字符串
	* @param iDefaultSize 格式错误时, 缺省的大小
	* @return             字节数
	*/
	static size_t toSize(const string &s, size_t iDefaultSize);

	/**
	* @brief  分隔字符串
	* @param s            待分隔的字符串
	* @param v 输出容器
	* @param sep 分隔字符串
	* @return             无
	*/
	static void strsplit(const std::string& s, std::vector<std::string>& v, const std::string& sep);

	/**
	* @brief  判断字符串是否以某个字符串开头
	* @param str            源字符串
	* @param prefix 开头内容
	* @return     bool
	*/
	bool start_with(const std::string &str, const std::string &prefix);

	/**
	* @brief  判断字符串是否以某个字符串结束
	* @param str            源字符串
	* @param prefix 结束内容
	* @return     bool
	*/
	bool end_with(const std::string &str, const std::string &suffix);
};

#endif

