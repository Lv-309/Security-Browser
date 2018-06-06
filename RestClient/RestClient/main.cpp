#include "Subject.h"
#include "RestClient.h"

#include <iostream>

int wmain()
{
	char* buffer = nullptr;
	size_t size = 0;

	const char file_path[] = "image.jpg";

	std::ifstream is(file_path);

	if (!is.is_open())
	{
		std::wcout << "Can't open file" << std::endl;
		is.close();
		return -1;
	}
	
	buffer = new char[size];

	is.read(buffer, size);
	is.close();



	Subject subj;

	RestClient client(&subj);	

	subj.setBufferData(buffer);	



	std::wcout << "Enter any key to continue..." << std::endl;
	std::cin.get();

	return 0;
}