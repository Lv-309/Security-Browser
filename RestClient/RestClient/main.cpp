#include <pplx/pplxtasks.h>

#include "HttpClient.h"

#include <iostream>
#include <string>

unsigned long GetSize(std::ifstream &is)
{
	long p = (long)is.tellg();
	is.seekg(0, std::ios::end);
	long size = (long)is.tellg();
	is.seekg(p);

	return size;
}

int wmain()
{
	
	//char* buffer = nullptr;
	//size_t size = 0;
	//const char file_path[] = "E:\\SoftServeCourse\\Rest_Client\\RestClient\\RestClient\\image.jpeg";

	//std::ifstream is;
	//is.open(file_path, std::ios::binary);

	//if (!is.is_open())
	//{
	//	std::wcout << "Can't open file" << std::endl;
	//	is.close();
	//	return -1;
	//}

	//size = GetSize(is);
	//buffer = new char[size];

	//is.read(buffer, size);
	//is.close();	

	//
	//HttpClient::HttpClient client;

	//pplx::task<void> requestTask = client.request_files_upload(buffer,size);

	//try
	//{
	//	requestTask.wait();
	//}
	//catch (const std::exception &e)
	//{
	//	std::cout << "Error exception: " << e.what() << std::endl;
	//}	
	//

	HttpClient::HttpClient client;

	pplx::task<void> requestTask = client.core_auth_request_password_reset();

	try
	{
		requestTask.wait();
	}
	catch (const std::exception &e)
	{
		std::cout << "Error exception: " << e.what() << std::endl;
	}
	

	std::wcout << "Enter any key to continue..." << std::endl;
	std::cin.get();

	//delete[] buffer;

	return 0;
}