#pragma once
#include "File.h"
#include "Severity.h"

namespace ISXLog
{
	class Log
	{
	public:
		inline Log(Severity _severity_level)
			: severity_level(_severity_level)
		{ }
		template <typename T>
		inline auto& operator<<(const T& msg);
	protected:
		File		file;
		Severity	severity_level;
	};

	template <typename T>
	inline auto& Log::operator<<(const T& msg)
	{
		file.OpenFile();
		file.WriteToFile(msg, severity_level);
		file.CloseFile();
		return *this;
	}
}