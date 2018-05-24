#pragma once
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <cpprest/filestream.h>
#include <pplx/pplxtasks.h>
#include <cpprest/filestream.h>
#include <cpprest/base_uri.h>

using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency;
using namespace concurrency::streams;

namespace HttpClient {	

	class HttpClient
	{

	public:
		HttpClient();		
		HttpClient(HttpClient&&);
		~HttpClient();

		HttpClient& operator=(HttpClient&&);

		pplx::task<void> request_get(const uri_builder&, std::shared_ptr<streams::ostream>&, const std::wstring& file_name = L"resaults");
		pplx::task<void> request_files_upload(char*, size_t );
		pplx::task<void> core_auth_request_password_reset();

	private:
		uri base_url;
		http_client client;		
	};
	
}

