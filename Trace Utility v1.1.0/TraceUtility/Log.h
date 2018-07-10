#pragma once
#include <mutex>
#include "File.h"
#include "Severity.h"

namespace ISXLog
{
	class Log
	{
	public:
		Log(Severity _severity_level) : severity_level(_severity_level) {}
		template <typename T>
		auto& operator<< (const T& msg);
	protected:
		ISXFile::File m_file;
		Severity severity_level;
	};

	template <typename T>
	auto& Log::operator<< (const T& msg)
	{
		this->m_file.OpenFile();
		this->m_file.WriteToFile(msg, severity_level);
		this->m_file.CloseFile();

		return *this;
	}
}

extern ISXLog::Log tlf;
