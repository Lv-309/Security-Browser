#include <vector>
#include <string>
#include <fstream>

#include "HttpClient.h"
#include "Base64.h" // for image decoding
//#include "LogInfo.h"

namespace ISXHttpClient 
{
	HttpClient::HttpClient() : token(L""), domen(L""), base_url(L"http://192.168.195.180"), client(base_url)
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

	pplx::task<void> HttpClient::request_files_upload(char* buffer, size_t lenght, utility::string_t filename)
	{		
		std::string encoded_str = base64_encode(reinterpret_cast<const unsigned char*>(buffer), lenght);
		std::wstring wstr_encoded(encoded_str.begin(), encoded_str.end());
				
		if (!domen.empty())
		{
			http_client new_client(domen);
			client = std::move(new_client);
		}

		std::wstring body = L"/webservice/rest/server.php?&wstoken=";
			
		body.append(token);		
		body.append(L"&wsfunction=core_files_upload&contextid=0&component=user&filearea=draft&itemid=0&filepath=/&filename=");
			
		body.append(filename);
		body.append(L"&filecontent=");
		
		body.append(wstr_encoded);
		body.append(L"&contextlevel=user&instanceid=7");				
		
		return client.request(methods::POST, body).then([](http_response response)
		{	
			if (response.status_code() == status_codes::OK)
			{
				//tlf_i << AT << "Server returned status code 200";
			}			
			
			// output on a consol
			std::wostringstream ss;
			ss << L"Server returned returned status code " << response.status_code() << L'.' << std::endl;			
			std::wcout << ss.str();
		
			// retrieve a response
			size_t lenght = (size_t)response.headers().content_length();
			istream bodyStream = response.body();

			container_buffer<std::string> inStringBuffer;

			return bodyStream.read(inStringBuffer, lenght).then([inStringBuffer](size_t bytesRead)
			{
				std::string &text = inStringBuffer.collection();
				std::wstring wstr(text.begin(), text.end());

				std::wcout << wstr << std::endl;				

				//tlf_i << AT << "Received responce";
			});
		});	
	}

	bool HttpClient::openConfigFile(std::string fileName)
	{
		std::ifstream in_file(fileName);

		if (!in_file.is_open())
		{
			std::wcout << "Config file is not opened" << std::endl;
			in_file.close();
			return false;
		}

		else
		{
			std::string line;

			while (std::getline(in_file, line))
			{
				if (line.find("#TOKEN") != std::string::npos)
				{
					line.erase(line.cbegin(), line.cbegin() + 7);
					std::wstring temp(line.cbegin(), line.cend());
					
					token.assign(temp);

					std::wcout << token << std::endl;
				}

				if (line.find("#DOMEN") != std::string::npos)
				{
					line.erase(line.cbegin(), line.cbegin() + 7);
					std::wstring temp(line.cbegin(), line.cend());

					domen.assign(temp);

					std::wcout << domen << std::endl;
				}
			}
		}

		return true;
	}
}