#pragma once
#include "File.h"
#include "Severity.h"

class Log
{
public:
	Log()
	{
		severity_level = Severity::Unspecified;
	}
	template <typename T>
	auto &operator << (const T& msg)
	{
		file.OpenFile();
		file.WriteToFile(msg, severity_level);
		file.CloseFile();
		return *this;
	}
protected:
	File file;
	Severity severity_level;
}tlf;