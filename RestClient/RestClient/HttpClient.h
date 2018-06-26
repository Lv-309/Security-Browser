#pragma once
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <pplx/pplxtasks.h>
#include <cpprest/filestream.h>
#include <cpprest/base_uri.h>

using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency;
using namespace concurrency::streams;

namespace ISXHttpClient 
{
	class HttpClient
	{

	public:
		HttpClient();		
		HttpClient(HttpClient&&);
		~HttpClient();

		HttpClient& operator=(HttpClient&&);
		
		pplx::task<void> request_files_upload(char* buffer, size_t lenght, utility::string_t filename);	
		bool openConfigFile(std::string);

	private:
		uri base_url;
		http_client client;
		std::wstring token;
		std::wstring domen;
		
	};
	
}

