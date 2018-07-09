#include <iostream>

#include "RestClient.h"
#include <algorithm>
#include <regex>
//#include "LogError.h"

	RestClient::RestClient(Subject* obj) : Observer(obj)
	{}

	void RestClient::Request(void)
	{
		char* data = Observer::GetSubject()->GetBufferData();
		size_t size_buf = Observer::GetSubject()->GetSize();
		std::wstring file_name(Observer::GetSubject()->GetFileName());

		std::wcout << size_buf << std::endl;

		pplx::task<void> requestTask = client.RequestFilesUpload(data, size_buf, file_name);

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
			//tlf_e << AT << "WinHttpSendRequest: 12029 (No internet connection)";			
		}
	}

	void RestClient::SetConfigFile(std::string fileConfig)
	{
		client.OpenConfigFile(fileConfig);
	}

	void RestClient::Request(std::string  filepath)
	{
		utility::string_t temp(filepath.begin(), filepath.end());

		pplx::task<void> requestTask = client.RequestFilesUpload(temp);

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
			//tlf_e << AT << "WinHttpSendRequest: 12029 (No internet connection)";
		}
	}