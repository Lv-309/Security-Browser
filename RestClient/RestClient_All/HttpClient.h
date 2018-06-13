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
		HttpClient(utility::string_t, utility::string_t);
		HttpClient(HttpClient&&);
		~HttpClient();

		HttpClient& operator=(HttpClient&&);
		
		pplx::task<void> request_files_upload(char* buffer, std::size_t lenght);
		pplx::task<void> core_auth_request_password_reset();

	private:
		uri base_url;
		http_client client;		
		utility::string_t user_name;
		utility::string_t user_password;
	};
	
}

