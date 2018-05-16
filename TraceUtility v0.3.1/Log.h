#ifndef LOG_H
#define LOG_H

#include "File.h"

class Log
{
public:
	Log()
	{
		severity_level = Unspecified;
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

#endif