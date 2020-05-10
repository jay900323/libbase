//UrlParser : a url parsing class
//Function : break a complete url into scheme, host, port, path, query params and fragment
//Refer to RFC 3986 for implementation details: http://tools.ietf.org/html/rfc3986
//
//Author: Jinkai

#ifndef _ZX_URLPARSER_H_
#define _ZX_URLPARSER_H_

#include "zx_base.h"
#include <string>
#include <map>
#include <vector>
using namespace std;

namespace ZxHttp {
	class BASE_CLASS UrlParser
	{
	public:
		typedef map<string, string> QueryParamList;

		UrlParser();
		UrlParser(const string & url);
		virtual ~UrlParser();

		bool valid() const { return valid_; }
		string schema() const { return schema_; }
		string host() const { return host_; }
		unsigned int port() const { return port_; }
		string authority() const;
		string path() const { return path_; }
		string query() const { return query_; }
		string getParamValue(const string & field) const { return queryParams_[field]; }
		string fragment() const { return fragment_; };

		void parse(const string & url);
		void parseAuthorityString(const string & authorityStr, string & host, unsigned int * portNo);
		void parseQueryParams(const string & queryParams, QueryParamList & paramList);
	private:
		bool valid_;
		string schema_;
		string host_;
		unsigned int port_;
		string path_;
		mutable QueryParamList queryParams_;
		string query_;
		string fragment_;
	};

}

#endif