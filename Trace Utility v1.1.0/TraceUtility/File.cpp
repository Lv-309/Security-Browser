#include "File.h"

void ISXFile::File::OpenFile()
{
	std::lock_guard<std::recursive_mutex> lock(mutex);

	std::wstring cur_dir = std::experimental::filesystem::current_path().c_str();
	std::experimental::filesystem::create_directory("Log");

	this->m_ofile.open(cur_dir + L"\\Log\\info_file.log", std::ios_base::app);
}

void ISXFile::File::CloseFile()
{
	std::lock_guard<std::recursive_mutex> lock(mutex);

	if (this->m_ofile.is_open())
		this->m_ofile.close();
}

char* ISXFile::File::GetCurTime()
{
	std::lock_guard<std::recursive_mutex> lock(mutex);

	std::time_t result = std::time(nullptr);
	return std::asctime(std::localtime(&result));
}

void ISXFile::File::EncodeWrite(std::string str)
{
	std::string encoded_str = base64_encode((const unsigned char*)str.c_str(), strlen(str.c_str()));
	this->m_ofile.write(encoded_str.c_str(), encoded_str.length());
	this->m_ofile.write("\n", strlen("\n"));
	encoded_str.clear();
}
