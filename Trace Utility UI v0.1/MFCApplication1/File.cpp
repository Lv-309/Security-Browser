#include "stdafx.h"
#include "File.h"

void File::OpenFile(std::string path)
{
	ifile.open(path);
}

void File::CloseFile()
{
	if (ifile.is_open())
		ifile.close();
}

std::vector<std::string> File::ReadFromFile()
{
	std::string line;
	std::vector<std::string> vec;

	while (std::getline(ifile, line))
	{
		Split(line, '\t', vec);
	}

	return vec;
}

void File::Split(const std::string &s, char delim, std::vector<std::string> &elems)
{
	std::stringstream ss;
	ss.str(s);
	std::string item;

	while (std::getline(ss, item, delim))
	{
		elems.push_back(item);
	}
}
