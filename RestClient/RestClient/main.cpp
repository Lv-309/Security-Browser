#include "Subject.h"
#include "RestClient.h"

#include <iostream>

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
	char* buffer = nullptr;
	size_t size = 0;

	const char file_path[] = "image2.jpg";

	std::ifstream is(file_path);

	if (!is.is_open())
	{
		std::wcout << "Can't open file" << std::endl;
		system("pause");
		is.close();
		return -1;
	}
	
	size = GetSize(is);
	buffer = new char[size];

	is.read(buffer, size);
	is.close();

	Subject subj;
	RestClient client(&subj);	

	client.SetConfigFile("config.txt");
	subj.SetBufferData(buffer, size, L"image2.jpg");

	std::wcout << "Enter any key to continue..." << std::endl;
	std::cin.get();

	return 0;
}