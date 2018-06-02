#include "File.h"

void File::OpenFile()
{
	this->m_ofile.open("info_file.log", std::ios_base::app);
}

void File::CloseFile()
{
	if (this->m_ofile.is_open())
		this->m_ofile.close();
}

char* File::GetCurrentTime()
{
	std::time_t result = std::time(nullptr);
	return std::asctime(std::localtime(&result));
}
