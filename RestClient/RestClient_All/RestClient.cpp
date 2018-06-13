#include <iostream>

#include "RestClient.h"

	RestClient::RestClient(Subject* obj) : Observer(obj)
	{}

	void RestClient::request()
	{
		char* data = Observer::getSubject()->getBufferData();

		std::size_t size_buf = Observer::getSubject()->getSize();

		std::wcout << size_buf << std::endl;

		pplx::task<void> requestTask = client.request_files_upload(data, size_buf);

		try
		{
			requestTask.wait();
		}

		catch (const std::exception &e)
		{
			std::wcout << "Error exception:" << e.what() << std::endl;
		}
	}

	