#pragma once

#define _CRT_SECURE_NO_WARNINGS

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
#include "Base64.h"

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

		this->m_counter_writing_to_file++;

		if (!(m_counter_writing_to_file % 2))
		{
			this->EncodeWrite((std::string)msg);
			this->EncodeWrite("\t");
		}
		else
		{
			if (severity_level == Severity::Unspecified)
			{
				this->EncodeWrite("[Unspecified]\t");
			}
			else if (severity_level == Severity::Trace)
			{
				this->EncodeWrite("[Trace]\t");
			}
			else if (severity_level == Severity::Debug)
			{
				this->EncodeWrite("[Debug]\t");
			}
			else if (severity_level == Severity::Info)
			{
				this->EncodeWrite("[Info]\t");
			}
			else if (severity_level == Severity::Warning)
			{
				this->EncodeWrite("[Warning]\t");
			}
			else if (severity_level == Severity::Error)
			{
				this->EncodeWrite("[Error]\t");
			}
			else if (severity_level == Severity::Fatal)
			{
				this->EncodeWrite("[Fatal]\t");
			}

			auto current_time = this->GetCurTime();
			this->EncodeWrite((std::string)current_time);
			this->EncodeWrite("\t");

			this->EncodeWrite((std::string)msg);
			this->EncodeWrite("\t");
		}
	}
}
