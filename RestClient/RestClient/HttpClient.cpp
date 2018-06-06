#include <vector>
#include <string>

#include "HttpClient.h"
#include "Base64.h" // for image decoding
#include "LogInfo.h"

namespace ISXHttpClient 
{
	HttpClient::HttpClient() : base_url(L"http://192.168.195.180"), client(base_url), 
		user_name(L""), user_password(L"")
	{
	}

	HttpClient::HttpClient(utility::string_t user_name, utility::string_t user_password) : user_name(user_name), user_password(user_password),
		base_url(L"http://192.168.195.180"), client(base_url)
	{
	}

	HttpClient::HttpClient(HttpClient&& rhs) : base_url(rhs.base_url), client(rhs.client)
	{
		// set rhs in deleted state
	}

	HttpClient::~HttpClient()
	{
	}

	HttpClient& HttpClient::operator=(HttpClient&& rhs)
	{
		if (this != &rhs)
		{
			// free resources, nothing (no pointers)

			base_url = rhs.base_url;
			client = rhs.client;
		}

		// set rhs in deleted state, nothing (no pointers)

		return *this;
	}	

	pplx::task<void> HttpClient::core_auth_request_password_reset()
	{	
		//moodlewsrestformat=json
		std::wstring str = L"/webservice/rest/server.php?&wstoken=859b2244c55636be03408c2b0e208b03&wsfunction=core_auth_request_password_reset&username=ws_access&email=taras.serhii@gmail.com";
					
			return client.request(
				methods::GET, str).then([](http_response response)
			{		
				std::wostringstream ss;
				ss << L"Server returned returned status code " << response.status_code() << L'.' << std::endl;
				std::wcout << ss.str();
		
				size_t lenght = (size_t)response.headers().content_length();		
				
				istream bodyStream = response.body();
		
				container_buffer<std::string> inStringBuffer;
				return bodyStream.read(inStringBuffer, lenght).then([inStringBuffer](size_t bytesRead)
				{
					std::string &text = inStringBuffer.collection();
					std::wstring wstr(text.begin(),text.end());
		
					std::wcout << wstr << std::endl;			
				});		
			});
	}

	pplx::task<void> HttpClient::request_files_upload(char* buffer, size_t lenght)
	{		
		std::string encoded_str = base64_encode(reinterpret_cast<const unsigned char*>(buffer), lenght);
		std::wstring wstr_encoded(encoded_str.begin(), encoded_str.end());		

		std::wstring body = L"/webservice/rest/server.php?&wstoken=859b2244c55636be03408c2b0e208b03&wsfunction=core_files_upload&contextid=0&component=user&filearea=draft&itemid=0&filepath=/&filename=photo1212.jpg&filecontent=";
		
		body.append(wstr_encoded);

		body.append(L"&contextlevel=user&instanceid=7");				
		
		return client.request(methods::POST, body).then([](http_response response)
		{	
			std::wostringstream ss;
			ss << L"Server returned returned status code " << response.status_code() << L'.' << std::endl;
			
			std::wcout << ss.str();

			std::string out(ss.str().begin(), ss.str().end());
			const char* info = out.c_str();

			tlf_i << AT << info;

			size_t lenght = (size_t)response.headers().content_length();

			istream bodyStream = response.body();

			container_buffer<std::string> inStringBuffer;

			return bodyStream.read(inStringBuffer, lenght).then([inStringBuffer](size_t bytesRead)
			{
				std::string &text = inStringBuffer.collection();
				std::wstring wstr(text.begin(), text.end());

				std::wcout << wstr << std::endl;				

				std::string out(wstr.begin(), wstr.end());
				const char* info = out.c_str();

				tlf_i << AT << info;
			});
		});
	}
}