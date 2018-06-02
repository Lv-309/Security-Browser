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
	int GetLineCount();
private:
	std::ifstream ifile;
	int line_counter = 0;

	void Split(const std::string &s, char delim, std::vector<std::string> &elems);
};
