#include "pch.h"
#include "mHttpClient.h"
#include <sstream>
#include <fstream>
#include <algorithm>
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
#include "Poco/Base64Encoder.h"




using namespace std;
using namespace rapidjson;
using Poco::Base64Encoder;


static const string base64_chars ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


mHttpClient::mHttpClient()
{
	curl_global_init(CURL_GLOBAL_ALL);
}


mHttpClient::~mHttpClient()
{
	curl_global_cleanup();
}


string mHttpClient::mDoPostRequest(const char* token,const char* filepath)
{
	//设置请求的URL地址
	curl = curl_easy_init();
	string strData,postData;
	string URL = "https://aip.baidubce.com/rest/2.0/image-classify/v2/advanced_general?access_token=";
	URL.append(token);
	curl_easy_setopt(curl,CURLOPT_URL,URL.c_str());
	//设置请求的方式 POST/GET...
	curl_easy_setopt(curl,CURLOPT_POST,1);
	//设置请求头信息
	curl_slist* HeaderList = nullptr;
	curl_slist_append(HeaderList,"Content-Type:application/x-www-form-urlencoded");
	curl_easy_setopt(curl,CURLOPT_HEADER,HeaderList);

	//设置要提交的Body数据
	postData.append("image=");
    postData.append(CrptImageData(filepath));
	curl_easy_setopt(curl,CURLOPT_POSTFIELDS,postData.data());
	curl_easy_setopt(curl,CURLOPT_POSTFIELDSIZE,postData.size());

	//设置忽略证书验证
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
	
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
	//设置返回数据的回调函数
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, mReturnDataCallBack);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strData);
	//设置请求连接超时时间
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5);
	//设置返回超时时间
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);

	CURLcode resp = curl_easy_perform(curl);

	if (resp==CURLE_OK)
	{
		
		curl_easy_cleanup(curl);
		return strData;
	}
	
	return "图片识别失败！";
}



string mHttpClient::mGetAccessToken(string appkey,string seckey)
{
	//设置请求的URL地址
	curl = curl_easy_init();
	string strData;
	string URL = "https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials";
	URL.append("&client_id=");
	URL.append(appkey);
	URL.append("&client_secret=");
	URL.append(seckey);
	curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
	//设置请求头信息
	curl_slist* HeaderList = nullptr;
	curl_slist_append(HeaderList, "Content-Type:application/json");
	curl_easy_setopt(curl, CURLOPT_HEADER, HeaderList);

	//设置忽略证书验证
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);

	curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
	//设置返回数据的回调函数
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, mReturnDataCallBack);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strData);
	//设置请求连接超时时间
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5);
	//设置返回超时时间
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);

	CURLcode resp = curl_easy_perform(curl);
	if (resp == CURLE_OK)
	{
		curl_easy_cleanup(curl);
		Document json;
		if (json.Parse(strData.c_str()).HasParseError())
		{
			return "解析Json失败！";
		}
		
		if (json.HasMember("access_token") && json["access_token"].IsString())
		{
			return json["access_token"].GetString();
		}

	}
	return "获取Token失败！";
}


//返回数据的回调函数
size_t mHttpClient::mReturnDataCallBack(void* buffer, size_t size, size_t nmemb, void* stream)
{
	string* DataStr = (string*)stream;
	DataStr->append((char*)buffer,size*nmemb);
	return size * nmemb;
}

//将图片Base64的实现
string mHttpClient::CrptImageData(string filepath)
{
	ifstream  imgfile(filepath.c_str(),ios::in|ios::binary);
	if (imgfile.is_open())
	{
		imgfile.seekg(0, imgfile.end);
		streampos slen = imgfile.tellg();
		int size = slen;
		char* buffer = (char*)malloc(sizeof(char)*size);
		string tmpstr = base64_encode(buffer, size);
		free(buffer);
		return UrlEncode(tmpstr);
	}
	return "";
}


std::string mHttpClient::base64_encode(const char * bytes_to_encode, unsigned int in_len)
{
	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (in_len--)
	{
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3)
		{
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;
			for (i = 0; (i <4); i++)
			{
				ret += base64_chars[char_array_4[i]];
			}
			i = 0;
		}
	}
	if (i)
	{
		for (j = i; j < 3; j++)
		{
			char_array_3[j] = '\0';
		}

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
		{
			ret += base64_chars[char_array_4[j]];
		}

		while ((i++ < 3))
		{
			ret += '=';
		}

	}
	return ret;
}



string mHttpClient::UrlEncode(const std::string& str)
{
	string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		if (isalnum((unsigned char)str[i]) ||
			(str[i] == '-') ||
			(str[i] == '_') ||
			(str[i] == '.') ||
			(str[i] == '~'))
			strTemp += str[i];
		else if (str[i] == ' ')
			strTemp += "+";
		else
		{
			strTemp += '%';
			strTemp += ToHex((unsigned char)str[i] >> 4);
			strTemp += ToHex((unsigned char)str[i] % 16);
		}
	}
	return strTemp;
}



unsigned char mHttpClient::ToHex(unsigned char x)
{
	return  x > 9 ? x + 55 : x + 48;
}