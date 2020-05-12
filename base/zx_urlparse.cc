#include "zx_urlparse.h"
#include "zx_base.h"
#include "zx_utils.h"
#include <iterator>
#include <sstream>
#include <algorithm>
#include <functional>


namespace ZxHttp {

	UrlParser::UrlParser() {
	}

	UrlParser::UrlParser(const string & url) :
		schema_(),
		host_(),
		port_(80),
		path_(),
		queryParams_(),
		fragment_()
	{
		parse(url);
	}

	UrlParser::~UrlParser()
	{
	}

	void UrlParser::parse(const string & url)
	{
		const string prot_end("://");
		string::const_iterator prot_i = search(url.begin(), url.end(),
			prot_end.begin(), prot_end.end());
		schema_.reserve(distance(url.begin(), prot_i));
		transform(url.begin(), prot_i,
			back_inserter(schema_),
			ptr_fun<int, int>(tolower));
		if (prot_i == url.end())
			return;
		advance(prot_i, prot_end.length());
		string::const_iterator path_i = find(prot_i, url.end(), '/');
		string authority;
		authority.reserve(distance(prot_i, path_i));

		transform(prot_i, path_i,
			back_inserter(authority),
			ptr_fun<int, int>(tolower));
		parseAuthorityString(authority, host_, &port_);
		string::const_iterator query_i = find(path_i, url.end(), '?');
		path_.assign(path_i, query_i);

		if (query_i != url.end())
			++query_i;
		string::const_iterator fragment_i = find(query_i, url.end(), '#');
		if (fragment_i != url.end())
		{
			query_.assign(query_i, fragment_i);
			++fragment_i;
			fragment_.assign(fragment_i, url.end());
		}
		else
		{
			query_.assign(query_i, url.end());
		}
		parseQueryParams(query_, queryParams_);
	}

	void UrlParser::parseAuthorityString(const string & authorityStr, string & host, unsigned int * portNo)
	{
		int positionOfAmp = authorityStr.find(":");
		host = authorityStr.substr(0, positionOfAmp);
		if (positionOfAmp != string::npos)
		{
			string portStr = authorityStr.substr(positionOfAmp + 1);
			*portNo = atoi(portStr.c_str());
		}
	}

	void UrlParser::parseQueryParams(const string & queryParams, QueryParamList & paramList)
	{
		vector<string> strs;

		ZxBase::strsplit(queryParams, strs, "&");
		for (vector<string>::iterator iter = strs.begin(); iter != strs.end(); ++iter)
		{
			string key, value;
			int positionOfEquals = iter->find("=");
			key = iter->substr(0, positionOfEquals);
			if (positionOfEquals != string::npos)
				value = iter->substr(positionOfEquals + 1);

			paramList.insert(std::pair<string, string>(key, value));
		}
	}

	string UrlParser::authority() const
	{
		stringstream s;
		s << host_ << ":" << port_;
		return s.str();
	}

}



