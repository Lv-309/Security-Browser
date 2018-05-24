#include "HttpClient.h"
#include "Base64.h" // for image encoding

namespace HttpClient {


	HttpClient::HttpClient() : base_url(L"http://192.168.195.180"), client(base_url)
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

	pplx::task<void> HttpClient::request_get(const uri_builder& query, std::shared_ptr<streams::ostream>& out_stream, const std::wstring& file_name)
	{
		std::wstring resault_file(file_name);
		resault_file += L".html";

		pplx::task<void> requestTask = streams::fstream::open_ostream(resault_file.c_str()).then([=](streams::ostream outFile)
		{
			*out_stream = outFile;

			uri_builder builder(query);

			return client.request(methods::GET, builder.to_string());
		})
			// Handle response headers arriving.
			.then([=](http_response response)
		{
			std::cout << "Received response status code: " << response.status_code() << std::endl;

			// Write response body into the file.
			return response.body().read_to_end(out_stream->streambuf());
		})

			// Close the file stream.
			.then([=](size_t)
		{
			return out_stream->close();
		});

		return requestTask;
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
		//std::string encoded_str = base64_encode(reinterpret_cast<const unsigned char*>(buffer), lenght);
		//std::wstring wstr_encoded(encoded_str.begin(), encoded_str.end());

		// moodlewsrestformat=json
		std::wstring fragment_path = L"/webservice/rest/server.php?&wstoken=859b2244c55636be03408c2b0e208b03&wsfunction=core_files_upload&component=user&filearea=draft&itemid=1&filepath=/&filename=image.jpg&filecontent=&contextlevel=course&instanceid=1";

		std::wstring body(&buffer[0],&buffer[lenght-1]);

		std::wcout << body << std::endl;

		return client.request(methods::POST,
			                  fragment_path,
			                  body).then([](http_response response)
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
				std::wstring wstr(text.begin(), text.end());

				std::wcout << wstr << std::endl;
			});
		});
	}
}