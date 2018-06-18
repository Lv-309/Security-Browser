#pragma once
#include <windows.h>
#include <fstream>
#include <sstream>
#include <vector>

class File
{
public:
	void OpenFile(std::string path);
	void CloseFile();
	std::vector<std::string> ReadFromFile();
private:
	std::ifstream ifile;
	void Split(const std::string &s, char delim, std::vector<std::string> &elems);
};
