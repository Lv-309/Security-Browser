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
#include "../base64l.h"

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

		std::string storage_string;
	};

	template <typename T>
	void File::WriteToFile(const T& msg, Severity severity_level)
	{
		std::lock_guard<std::recursive_mutex> lock(mutex);

		m_counter_writing_to_file++;

		storage_string.clear();

		if (!(m_counter_writing_to_file % 2))
		{
			storage_string += (std::string)msg;
			storage_string += "\t";
		}
		else
		{
			if (severity_level == Severity::Unspecified)
			{
				storage_string += "[Unspecified]\t";
			}
			else if (severity_level == Severity::Trace)
			{
				storage_string += "[Trace]\t";
			}
			else if (severity_level == Severity::Debug)
			{
				storage_string += "[Debug]\t";
			}
			else if (severity_level == Severity::Info)
			{
				storage_string += "[Info]\t";
			}
			else if (severity_level == Severity::Warning)
			{
				storage_string += "[Warning]\t";
			}
			else if (severity_level == Severity::Error)
			{
				storage_string += "[Error]\t";
			}
			else if (severity_level == Severity::Fatal)
			{
				storage_string += "[Fatal]\t";
			}

			auto current_time = GetCurTime();

			storage_string += (std::string)current_time;
			storage_string += "\t";
			storage_string += (std::string)msg;
			storage_string += "\t";
		}

		std::string encoded_storage_string;

		char chars[] = "\n";
		for (unsigned int i = 0; i < strlen(chars); ++i)
			storage_string.erase(std::remove(storage_string.begin(),
				storage_string.end(), chars[i]), storage_string.end());

		encoded_storage_string = base64l_encode((const unsigned char*)storage_string.c_str(),
			strlen(storage_string.c_str())/*storage_string.length()-1*/);
		this->m_ofile.write(encoded_storage_string.c_str(), encoded_storage_string.length());
	}
}
