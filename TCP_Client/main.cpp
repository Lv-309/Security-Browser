#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <string>
#include <fstream>
#include <winsock2.h>
#include <windows.h>

#pragma comment (lib, "Ws2_32.lib")

#define SOURCE_FILE_CONFIG "E:\\SV_Course\\Tasks\\TCP_Client\\config\\config.txt"

/* SOURCE CONFIG FILE EXEPMLE
*
*  #PORT 5938
*  #SERVERADDRESS 127.0.0.1
*  #FILEPATHDATA E:\\SV_Course\\Tasks\\TCP_Client\\image\\image.jpg
*
*/

struct ConfigData 
{
	int PORT;
	std::string SERVER_ADDRESS;
	std::string FILE_PATH_DATA;
};

unsigned long GetSize(std::ifstream &is);

bool SendParts(SOCKET socket, const int part_size, const int buffer_size, const char *buffer);

bool GetConfig(const char* sourse_file_data, ConfigData& data);

int main()
{
	ConfigData config_data = { 0,"",""};

	bool res = GetConfig(SOURCE_FILE_CONFIG, config_data);

	if (!res)
	{
		return -1;
	}

	const int PORT = config_data.PORT;
	const char* SERVER_ADDRESS = config_data.SERVER_ADDRESS.c_str();
	const char* FILE_PATH_DATA = config_data.FILE_PATH_DATA.c_str();

	WSADATA ws_data;
	SOCKET connect_socket;
	sockaddr_in destin_addr;	

	std::cout << "TCP DEMO CLIENT" << std::endl;

	int result = WSAStartup(MAKEWORD(2, 2), &ws_data);

	if (result != 0)
	{
		std::cout << "<TCP CLIENT> Error Initialization Win sock: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return -1;
	}

	connect_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (connect_socket < 0)
	{
		std::cout << "<TCP CLIENT> Error creation socket: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return -1;
	}

	destin_addr.sin_family = AF_INET;
	destin_addr.sin_port = htons(PORT);
	
	if (inet_addr(SERVER_ADDRESS) != INADDR_NONE)
		destin_addr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
	
	if (connect(connect_socket, (sockaddr *)&destin_addr, sizeof(destin_addr)))
	{
		std::cout << "<TCP CLIENT> Error connection: " << WSAGetLastError() << std::endl;
		closesocket(connect_socket);
		WSACleanup();
		return -1;
	}

	std::cout << "<TCP CLIENT> Connected with " << SERVER_ADDRESS << std::endl;
	
	std::ifstream is;
	is.open(FILE_PATH_DATA, std::ios::in | std::ios::binary);

	int buffer_size = GetSize(is);
	char *buffer = new char[buffer_size];
	is.read(buffer, buffer_size);
	is.close();

	if (is.fail()) 
	{
		std::cerr << "<TCP CLIENT> Error file opening" << std::endl;
		return -1;
	}		

	std::cout << "<TCP CLIENT> Successful loaded image ... \n" << std::endl;

	if (!SendParts(connect_socket, 1024, buffer_size, buffer))
	{
		std::cout << "<TCP CLIENT> Error sending ... \n" << std::endl;
		return -1;
	}

	std::cout << "<TCP CLIENT> Successful sent ... \n" << std::endl;	
	
	delete[] buffer;

	closesocket(connect_socket);
	WSACleanup();

	return 0;
}

unsigned long GetSize(std::ifstream &is)
{
	long p = (long)is.tellg();
	is.seekg(0, std::ios::end);
	long size = (long)is.tellg();
	is.seekg(p);

	return size;
}

bool SendParts(SOCKET socket, const int part_size, const int buffer_size, const char *buffer)
{
	int result = send(socket, (char *)&buffer_size, sizeof(buffer_size), 0);

	if (result == SOCKET_ERROR)
		return false;

	int parts_count = buffer_size / part_size;
	int last_part_size = buffer_size % part_size;

	for (int i = 0; i < parts_count; i++)
	{
		result = send(socket, buffer, part_size, 0);

		if (result == SOCKET_ERROR)
			return false;

		buffer += part_size;
	}

	result = send(socket, buffer, last_part_size, 0);

	if (last_part_size && (result == SOCKET_ERROR))
		return false;

	return true;
}

bool  GetConfig(const char* sourse_file_data, ConfigData& data)
{
	std::ifstream is(sourse_file_data);

	if (!is.is_open())
	{
		std::cerr << "<ERROR> Can't open config source file" << std::endl;
		is.close();
		return false;
	}

	if (is.peek() == is.eof())
	{
		std::cerr << "<ERROR> File is empty" << std::endl;
		is.close();
		return false;
	}

	std::string line;

	while (std::getline(is, line))
	{
		if (line.find("#PORT") != std::string::npos)
		{			
			line.erase(line.cbegin(), line.cbegin() + 6);
			data.PORT = atoi(line.c_str());

			//std::cout << data.PORT << std::endl;
		}

		if (line.find("#SERVERADDRESS") != std::string::npos)
		{
			line.erase(line.cbegin(), line.cbegin() + 15);
			data.SERVER_ADDRESS = line.c_str();

			//std::cout << data.SERVER_ADDRESS << std::endl;
		}

		if (line.find("#FILEPATHDATA") != std::string::npos)
		{
			line.erase(line.cbegin(), line.cbegin() + 14);
			data.FILE_PATH_DATA = line.c_str();

			//std::cout << data.FILE_PATH_DATA << std::endl;
		}
	}

	is.close();

	return true;
}