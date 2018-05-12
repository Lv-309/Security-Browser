#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <string>
#include <fstream>
#include <winsock2.h>
#include <windows.h>

#pragma comment (lib, "Ws2_32.lib")

#define PORT 5938
#define OUTPUT_FILE_PATH "E:\\SV_Course\\Tasks\\Server\\image.jpg"

int g_nclients = 0;

DWORD WINAPI ProcessClient(LPVOID client_socket);

bool RecvParts(SOCKET socket, const int part_size, int * const buffer_size, char ** const buffer)
{
	int result = recv(socket, (char *)buffer_size, sizeof(*buffer_size), 0);

	if (result == SOCKET_ERROR)
	{
		*buffer_size = 0;
		*buffer = 0;

		return false;
	}

	*buffer = new char[*buffer_size];

	char *p_buf = *buffer;

	int parts_count = *buffer_size / part_size;
	int last_part_size = *buffer_size % part_size;

	for (int i = 0; i < parts_count; i++)
	{
		result = recv(socket, p_buf, part_size, 0);

		if (result == SOCKET_ERROR)
		{
			*buffer_size = 0;
			*buffer = 0;
			delete *buffer;	

			return false;
		}
		p_buf += part_size;
	}

	result = recv(socket, p_buf, last_part_size, 0);

	if (last_part_size && (result == SOCKET_ERROR))
	{
		*buffer_size = 0;		
		*buffer = 0;

		delete *buffer;
		return false;
	}

	return true;
}

int main()
{
	WSADATA ws_data;
	SOCKET connect_socket;
	sockaddr_in local_addr;
	SOCKET client_socket;
	sockaddr_in client_addr;
	HOSTENT *hst;
	DWORD this_id;

	std::cout << "... Server DEMO ..." << std::endl;

	int result = WSAStartup(MAKEWORD(2, 2), &ws_data);

	if (result != 0)
	{
		std::cout << "... Error Initialization Win sock: " << WSAGetLastError() << std::endl;			
		return -1;
	}

	connect_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (connect_socket == SOCKET_ERROR)
	{
		std::cout<<"... Error connection socket: " << WSAGetLastError()<<std::endl;
		WSACleanup();
		return -1;
	}

	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(PORT);
	local_addr.sin_addr.s_addr = 0;

	result = bind(connect_socket, (sockaddr*)&local_addr, sizeof(local_addr));

	if (result == SOCKET_ERROR)
	{
		std::cout << "... Error binding: " << WSAGetLastError() << std::endl;
		closesocket(connect_socket);
		WSACleanup();
		return -1;
	}

	result = listen(connect_socket, SOMAXCONN);

	if (result == SOCKET_ERROR)
	{
		std::cout << "... Error listening: " << WSAGetLastError() << std::endl;
		closesocket(connect_socket);
		WSACleanup();
		return -1;
	}

	std::cout << "<SERVER> Wait for connecting " << std::endl;

	int client_size = sizeof(client_addr);	

	while ((client_socket = accept(connect_socket, (sockaddr*)&client_addr, &client_size)))
	{
		g_nclients++;

		hst = gethostbyaddr((char *)&client_addr.sin_addr.s_addr, 4, AF_INET);

		const char* host_name = (hst) ? hst->h_name : nullptr;
		const char* host_addr = inet_ntoa(client_addr.sin_addr);

		std::cout << "... HOST " << host_name << " HOS ADDRESS " << host_addr << " ... " << std::endl;

		if (g_nclients)
			std::cout << g_nclients << " <SERVER> User on-line " << std::endl;
		else
			std::cout << "<SERVER> No User on line \n" << std::endl;

		CreateThread(NULL, NULL, ProcessClient, &client_socket, NULL, &this_id);
	}
	return 0;
}

DWORD WINAPI ProcessClient(LPVOID client_socket)
{
	SOCKET sock;
	sock = ((SOCKET *)client_socket)[0];

	int buffer_size = 0;
	char *buffer = 0;

	bool result = RecvParts(sock, 1024, &buffer_size, &buffer);

	if (!result) 
		return -1;

	std::cout << "<SERVER> Receive successful \n" << std::endl;

	std::ofstream os;
	os.open(OUTPUT_FILE_PATH, std::ios::out | std::ios::binary);
	os.write(buffer, buffer_size);
	os.close();

	if (os.fail())
		return -1;

	std::cout << "<SERVER> Successful saved ... \n" << std::endl;

	delete[] buffer;	

	g_nclients--;

	std::cout << "<SERVER> Disconnect one user ..." << std::endl;

	if (g_nclients)
		std::cout << g_nclients << "<SERVER> User on-line " << std::endl;
	else
		std::cout << "<SERVER> No User on line\n" << std::endl;

	closesocket(sock);
	return 0;
}
