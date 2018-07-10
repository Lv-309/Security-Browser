#pragma once

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)

#include <mutex>
#include <fstream>
#include <ctime>
#include <vector>
#include <algorithm>
#include <filesystem>
#include "Severity.h"
#include "../TraceUtility/base64l.h"

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
		void EncodeWrite(std::string);
	};

	template <typename T>
	void File::WriteToFile(const T& msg, Severity severity_level)
	{
		std::lock_guard<std::recursive_mutex> lock(mutex);

		m_counter_writing_to_file++;

		if (!(m_counter_writing_to_file % 2))
		{
			EncodeWrite((std::string)msg);
			EncodeWrite("\t");
		}
		else
		{
			if (severity_level == Severity::Unspecified)
			{
				EncodeWrite("[Unspecified]\t");
			}
			else if (severity_level == Severity::Trace)
			{
				EncodeWrite("[Trace]\t");
			}
			else if (severity_level == Severity::Debug)
			{
				EncodeWrite("[Debug]\t");
			}
			else if (severity_level == Severity::Info)
			{
				EncodeWrite("[Info]\t");
			}
			else if (severity_level == Severity::Warning)
			{
				EncodeWrite("[Warning]\t");
			}
			else if (severity_level == Severity::Error)
			{
				EncodeWrite("[Error]\t");
			}
			else if (severity_level == Severity::Fatal)
			{
				EncodeWrite("[Fatal]\t");
			}

			auto current_time = GetCurTime();
			EncodeWrite((std::string)current_time);
			EncodeWrite("\t");

			EncodeWrite((std::string)msg);
			EncodeWrite("\t");
		}
	}
}
