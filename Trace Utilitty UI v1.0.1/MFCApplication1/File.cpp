#include "stdafx.h"
#include "File.h"
#include "base64.h"

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
	std::string encoded_data = "";
	std::string decoded_data = "";
	this->m_vect.clear();

	std::vector<std::string> dec_vec;

	while (std::getline(ifile, line))
	{
		encoded_data = line;
		dec_vec.push_back(base64_decode(encoded_data));
		encoded_data.clear();
	}

	for (auto v : dec_vec)
	{
		decoded_data += v;
	}

	Split(decoded_data, '\t', this->m_vect);

	return this->m_vect;
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
