#pragma once

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)

#include <mutex>
#include <fstream>
#include <ctime>
#include <filesystem>
#include "Severity.h"

namespace ISXFile
{
	class File
	{
	public:
		void OpenFile();
		void CloseFile();
		template <typename T>
		void WriteToFile(const T& msg, Severity severity_level);
	private:
		std::ofstream m_ofile;
		int m_counter_writing_to_file = 0;
		std::recursive_mutex mutex;
		char* GetCurTime();
	};

	template <typename T>
	void File::WriteToFile(const T& msg, Severity severity_level)
	{
		std::lock_guard<std::recursive_mutex> lock(mutex);

		m_counter_writing_to_file++;

		if (!(m_counter_writing_to_file % 2))
		{
			this->m_ofile.write(msg, strlen(msg));
			this->m_ofile.write("\n\n", sizeof(""));
		}
		else
		{
			if (severity_level == Severity::Unspecified)
				this->m_ofile.write("[Unspecified]\t", sizeof("[Unspecified]"));
			else if (severity_level == Severity::Trace)
				this->m_ofile.write("[Trace]\t", sizeof("[Trace]"));
			else if (severity_level == Severity::Debug)
				this->m_ofile.write("[Debug]\t", sizeof("[Debug]"));
			else if (severity_level == Severity::Info)
				this->m_ofile.write("[Info]\t", sizeof("[Info]"));
			else if (severity_level == Severity::Warning)
				this->m_ofile.write("[Warning]\t", sizeof("[Warning]"));
			else if (severity_level == Severity::Error)
				this->m_ofile.write("[Error]\t", sizeof("[Error]"));
			else if (severity_level == Severity::Fatal)
				this->m_ofile.write("[Fatal]\t", sizeof("[Fatal]"));

			auto current_time = GetCurTime();
			this->m_ofile.write(current_time, 24);
			this->m_ofile.write("\t", sizeof(""));

			this->m_ofile.write(msg, strlen(msg));
			this->m_ofile.write("\t", sizeof(""));
		}
	}
}
