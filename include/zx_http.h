#ifndef _ZX_HTTP_H_
#define _ZX_HTTP_H_

#include "zx_base.h"
#include <memory>
#include <map>

using namespace std;

namespace ZxHttp {

	class BackendResponse;

	namespace detail {

		struct ci {
			bool operator()(const std::string &s1, const std::string &s2) const {
				return std::lexicographical_compare(
					s1.begin(), s1.end(), s2.begin(), s2.end(),
					[](char c1, char c2) { return ::tolower(c1) < ::tolower(c2); });
			}
		};

		class Buffer
		{
		public:
			Buffer(std::string buffer);
			bool start_with_crlf();
			std::string get_next_segment(std::string separator);
		private:
		private:
			std::string _buffer;
			int _currentPosition;
		};
	}

	using Headers = std::multimap<std::string, std::string, detail::ci>;

	class BASE_CLASS BackendRequest
	{

	private:
		void *curl = nullptr;
		unsigned int curl_code;
		const std::string url;
		const char *post_data;
		Headers headers;

	public:
		BackendRequest(const std::string &url, const char *post_data);
		virtual ~BackendRequest();
		std::shared_ptr<BackendResponse> curl_perform(int timeout);
		unsigned int get_curl_code() const;
		const char *get_curl_err_msg() const;
		void set_header(const char *key, const char *val);
		bool has_header(const char *key) const;
		std::string get_header_value(const char *key, size_t id) const;
	};

	class BASE_CLASS BackendResponse
	{
	public:
		bool parse_error = false;
		long response_code = 0;
		std::string header_string;
		std::string response_string;
		std::string error_msg;
		Headers headers;

	public:
		BackendResponse(long response_code, std::string header_string, std::string response_string);
		void parse_http_headers(std::string &header_str);
		bool has_error() const;
		bool http_code_ok() const;
		long get_http_code() const;
		std::string to_string() const;
		void set_header(const char *key, const char *val);
	};

};

#endif