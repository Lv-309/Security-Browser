#pragma once

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)

#include <fstream>
#include <ctime>

#include "Severity.h"

static int i = 0;

class File
{
public:
	void OpenFile();
	void CloseFile();
	template <typename T>
	void WriteToFile(const T& msg, Severity severity_level);
private:
	std::ofstream m_ofile;
	char* GetCurTime();
}; 


template <typename T>
void File::WriteToFile(const T& msg, Severity severity_level)
{
	i++;

	if (!(i % 2))
	{
		this->m_ofile.write(msg, sizeof(msg));
		this->m_ofile.write("\n\n", sizeof("\n\n"));
	}
	else
	{
		if (severity_level == Severity::Unspecified)
			this->m_ofile.write("[Unspecified]\t", sizeof("[Unspecified]\t"));
		else if (severity_level == Severity::Trace)
			this->m_ofile.write("[Trace]\t", sizeof("[Trace]\t"));
		else if (severity_level == Severity::Debug)
			this->m_ofile.write("[Debug]\t", sizeof("[Debug]\t"));
		else if (severity_level == Severity::Info)
			this->m_ofile.write("[Info]\t\t", sizeof("[Info]\t\t"));
		else if (severity_level == Severity::Warning)
			this->m_ofile.write("[Warning]\t", sizeof("[Warning]\t"));
		else if (severity_level == Severity::Error)
			this->m_ofile.write("[Error]\t", sizeof("[Error]\t"));
		else if (severity_level == Severity::Fatal)
			this->m_ofile.write("[Fatal]\t", sizeof("[Fatal]\t"));

		auto current_time = this->GetCurTime();
		this->m_ofile.write(current_time, 24);
		this->m_ofile.write("\t", sizeof("\t"));

		this->m_ofile.write(msg, sizeof(msg));
		this->m_ofile.write("\t", sizeof("\t"));
	}
}