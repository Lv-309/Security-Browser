#pragma once
#define _CRT_SECURE_NO_WARNINGS

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)

#include <iostream>
#include <fstream>
#include <ctime>

#include "Severity.h"

static int counter_write_to_file = 0;

class File
{
public:
	void OpenFile();
	void CloseFile();

	template <typename T>
	void WriteToFile(const T& msg, Severity severity_level)
	{
		counter_write_to_file++;

		if (!(counter_write_to_file % 2))
		{
			ofile.write(msg, sizeof(msg));
			ofile.write("\n", sizeof(""));
		}
		else
		{
			if (severity_level == Severity::Unspecified)
				ofile.write("[Unspecified]\t", sizeof("[Unspecified]"));
			else if (severity_level == Severity::Trace)
				ofile.write("[Trace]\t", sizeof("[Trace]"));
			else if (severity_level == Severity::Debug)
				ofile.write("[Debug]\t", sizeof("[Debug]"));
			else if (severity_level == Severity::Info)
				ofile.write("[Info]\t", sizeof("[Info]"));
			else if (severity_level == Severity::Warning)
				ofile.write("[Warning]\t", sizeof("[Warning]"));
			else if (severity_level == Severity::Error)
				ofile.write("[Error]\t", sizeof("[Error]"));
			else if (severity_level == Severity::Fatal)
				ofile.write("[Fatal]\t", sizeof("[Fatal]"));

			auto current_time = GetCurrentTime();
			ofile.write(current_time, 24);
			ofile.write("\t", sizeof(""));

			ofile.write(msg, sizeof(msg));
			ofile.write("\t", sizeof(""));
		}
	}
	
private:
	std::ofstream ofile;
	
	auto GetCurrentTime()
	{
		std::time_t result = std::time(nullptr);
		return std::asctime(std::localtime(&result));
	}
};