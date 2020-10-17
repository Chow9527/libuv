#pragma once

#include "curl/curl.h"
#include <string>

class mHttpClient
{
public:
	mHttpClient();
	~mHttpClient();
	std::string mDoPostRequest(const char* token, const char* filepath);
	std::string mGetAccessToken(std::string appkey, std::string seckey);
	
private:
	CURL* curl;
	std::string CrptImageData(std::string filepath);
	std::string base64_encode(const char * bytes_to_encode, unsigned int in_len);
	static size_t mReturnDataCallBack(void* buffer, size_t size, size_t nmemb, void* stream);
	std::string UrlEncode(const std::string& str);
	unsigned char ToHex(unsigned char x);
};

