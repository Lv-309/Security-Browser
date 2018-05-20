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
			ofile.write("\n\n", sizeof("\n\n"));
		}
		else
		{
			if (severity_level == Severity::Unspecified)
				ofile.write("[Unspecified]\t", sizeof("[Unspecified]\t"));
			else if (severity_level == Severity::Trace)
				ofile.write("[Trace]\t", sizeof("[Trace]\t"));
			else if (severity_level == Severity::Debug)
				ofile.write("[Debug]\t", sizeof("[Debug]\t"));
			else if (severity_level == Severity::Info)
				ofile.write("[Info]\t\t", sizeof("[Info]\t\t"));
			else if (severity_level == Severity::Warning)
				ofile.write("[Warning]\t", sizeof("[Warning]\t"));
			else if (severity_level == Severity::Error)
				ofile.write("[Error]\t", sizeof("[Error]\t"));
			else if (severity_level == Severity::Fatal)
				ofile.write("[Fatal]\t", sizeof("[Fatal]\t"));

			auto current_time = GetCurrentTime();
			ofile.write(current_time, 24);
			ofile.write("\t", sizeof("\t"));

			ofile.write(msg, sizeof(msg));
			ofile.write("\t", sizeof("\t"));
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
