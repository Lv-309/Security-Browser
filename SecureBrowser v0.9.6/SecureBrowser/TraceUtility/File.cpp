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
