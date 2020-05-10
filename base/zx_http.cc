#include "curl/curl.h"
#include "zx_http.h"
#include "zx_urlparse.h"

namespace ZxHttp
{
	namespace detail {

		inline bool has_header(const Headers &headers, const char *key) {
			return headers.find(key) != headers.end();
		}

		inline const char *get_header_value(const Headers &headers, const char *key,
			size_t id = 0, const char *def = nullptr) {
			auto it = headers.find(key);
			std::advance(it, id);
			if (it != headers.end()) { return it->second.c_str(); }
			return def;
		}

		Buffer::Buffer(std::string buffer)
		{
			_buffer = buffer;
			_currentPosition = 0;
		}

		bool Buffer::start_with_crlf()
		{
			std::string currentBuffer = _buffer.substr(_currentPosition, _buffer.size());
			if (!strncmp(currentBuffer.c_str(), "\r\n", 2)) {
				_currentPosition += 2;
				return true;
			}
			return false;
		}

		std::string Buffer::get_next_segment(std::string separator)
		{
			std::string currentBuffer = _buffer.substr(_currentPosition, _buffer.size());
			int position = currentBuffer.find(separator);
			if (position == std::string::npos) {
				return "NA";
			}

			std::string segment = currentBuffer.substr(0, position);
			_currentPosition += (position + separator.length());

			return segment;
		}

	}

	static size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string *data)
	{
		data->append((char *)ptr, size * nmemb);
		return size * nmemb;
	}

	bool BackendRequest::has_header(const char *key) const  {
		return detail::has_header(headers, key);
	}

	std::string BackendRequest::get_header_value(const char *key, size_t id) const {
		return detail::get_header_value(headers, key, id, "");
	}

	BackendRequest::BackendRequest(const std::string &url, const char *post_data)
		: url(url), post_data(post_data)
	{
		curl = curl_easy_init();
		curl_code = CURL_LAST;
	}

	BackendRequest::~BackendRequest()
	{
		if (nullptr != curl)
		{
			curl_easy_cleanup(curl);
			curl = nullptr;
		}
	}

	std::shared_ptr<BackendResponse> BackendRequest::curl_perform(int timeout)
	{
		if (curl)
		{
			long response_code;
			std::string header_string;
			std::string response_string;
			struct curl_slist *chunk = nullptr;

			for (const auto &x : headers) {
				chunk = curl_slist_append(chunk, (x.first + ": " + x.second).c_str());
			}
			curl_code = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
			curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
			curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

#if !defined(_WIN32)
			curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
#endif

			if (post_data)
			{
				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
			}
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &(response_string));
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, &(header_string));
			curl_code = curl_easy_perform(curl);
			if (CURLE_OK == curl_code)
			{
				curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &(response_code));
				return std::make_shared<BackendResponse>(response_code, header_string, response_string);
			}
		}
		return nullptr;
	}

	void BackendRequest::set_header(const char *key, const char *val)
	{
		headers.emplace(key, val);
	}

	unsigned int BackendRequest::get_curl_code() const
	{
		return curl_code;
	}

	const char *BackendRequest::get_curl_err_msg() const
	{
		return curl_easy_strerror((CURLcode)curl_code);
	}

	BackendResponse::BackendResponse(long code, std::string headerstr, std::string response_str)
	{
		response_code = code;
		header_string = headerstr;
		response_string = response_str;
		parse_http_headers(headerstr);
	}

	void BackendResponse::parse_http_headers(std::string &header_str) {
		std::string key, value;
		detail::Buffer buffer(header_str);
		while (true) {
			key = buffer.get_next_segment(": ");
			if (key == "NA")
				break;
			value = buffer.get_next_segment("\r\n");
			set_header(key.c_str(), value.c_str());
			if (buffer.start_with_crlf())
				break;
		}
	}

	bool BackendResponse::has_error() const
	{
		return parse_error;
	}

	long BackendResponse::get_http_code() const
	{
		return response_code;
	}

	bool BackendResponse::http_code_ok() const
	{
		return (response_code >= 200 && response_code < 300);
	}

	void BackendResponse::set_header(const char *key, const char *val)
	{
		headers.emplace(key, val);
	}

	std::string BackendResponse::to_string() const
	{
		return "Response_code: " + std::to_string(response_code) + "\nheader_string: " + header_string + "body:" + response_string;
	}
}