#pragma once
#include <windows.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

class File
{
public:
	void OpenFile(std::string path);
	void CloseFile();
	std::vector<std::string> ReadFromFile();
	std::vector<std::string> m_vect;
private:
	std::ifstream ifile;
	void Split(const std::string &s, char delim, std::vector<std::string> &elems);
};
