#pragma once
#include <mutex>
#include "File.h"
#include "Severity.h"

namespace ISXLog
{
	static std::mutex mtx;
	
	class Log
	{
	public:
		Log(Severity _severity_level) : severity_level(_severity_level) {}
		template <typename T>
		auto& operator<< (const T& msg);
	protected:
		File file;
		Severity severity_level;
	};

	template <typename T>
	auto& Log::operator<< (const T& msg)
	{
		mtx.lock();

		file.OpenFile();
		file.WriteToFile(msg, severity_level);
		file.CloseFile();

		mtx.unlock();

		return *this;
	}
}

extern ISXLog::Log tlf;
