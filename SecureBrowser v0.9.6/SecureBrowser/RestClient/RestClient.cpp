#include <iostream>

#include "RestClient.h"
#include <algorithm>
#include <regex>
#include "..\TraceUtility\LogError.h"

	RestClient::RestClient(Subject* obj) : Observer(obj)
	{}

	void RestClient::request()
	{
		char* data = Observer::getSubject()->getBufferData();
		size_t size_buf = Observer::getSubject()->getSize();
		std::wstring file_name(Observer::getSubject()->getFileName());

		std::wcout << size_buf << std::endl;
		pplx::task<void> requestTask = client.request_files_upload(data, size_buf, file_name);

		try
		{
			requestTask.wait();
		}
		catch (const std::invalid_argument &e)
		{
			std::wcout << e.what() << std::endl;
		}
		catch (const std::exception &e)
		{
			std::wcout << "Error exception: " << e.what() << std::endl;
			tlf_e << AT << "WinHttpSendRequest: 12029 (No internet connection)";			
		}
	}

	void RestClient::SetConfigFile(std::string fileConfig)
	{
		client.openConfigFile(fileConfig);
	}

	void RestClient::request(std::string  filepath)
	{
		utility::string_t temp(filepath.begin(), filepath.end());

		pplx::task<void> requestTask = client.request_files_upload(temp);

		try
		{
			requestTask.wait();
		}
		catch (const std::invalid_argument &e)
		{
			std::wcout << e.what() << std::endl;
		}
		catch (const std::exception &e)
		{
			std::wcout << "Error exception: " << e.what() << std::endl;
			tlf_e << AT << "WinHttpSendRequest: 12029 (No internet connection)";
		}
	}