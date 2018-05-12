#pragma once
#define _CRT_SECURE_NO_WARNINGS

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <ctime>

enum Mode { Standard, Error, Warning };

static int i = 0;

class File
{
public:
	void OpenFile()
	{
		ofile.open("info_file.log", std::ios_base::app);
	}
	void CloseFile()
	{
		if (ofile.is_open())
			ofile.close();
	}
	template <typename T>
	void WriteToFile(const T& msg, Mode mode)
	{
		i++;

		if (!(i % 2))
		{
			if (mode == Standard)
				ofile.write("Default\t", sizeof("Default\t"));
			else if (mode == Error)
				ofile.write("Error\t\t", sizeof("Error\t\t"));
			else if (mode == Warning)
				ofile.write("Warning\t", sizeof("Warning\t"));
			
			ofile.write(msg, sizeof(msg));
			ofile.write("\t", sizeof("\t"));
			
			std::time_t result = std::time(nullptr);
			ofile.write("Date: ", sizeof("Date: "));
			ofile.write(std::asctime(std::localtime(&result)), 25);
		}
		else
		{
			ofile.write(msg, sizeof(msg));
			ofile.write("\t", sizeof("\t"));
		}
	}
private:
	std::ofstream ofile;

	bool FileExists(const std::string& name)
	{
		struct stat buffer;
		return (stat(name.c_str(), &buffer) == 0);
	}
};

class Log
{
public:
	template <typename T>
	Log & operator << (const T& msg)
	{
		file.OpenFile();
		file.WriteToFile(msg, mode);
		file.CloseFile();
		return *this;
	}
protected:
	File file;
private:
	Mode mode = Standard;
}tlf;

class LogError : public Log
{
public:
	template <typename T>
	LogError & operator << (const T& msg)
	{
		file.OpenFile();
		file.WriteToFile(msg, mode_error);
		file.CloseFile();
		return *this;
	}
private:
	Mode mode_error = Error;
}tlf_e;

class LogWarning : public Log
{
public:
	template <typename T>
	LogWarning & operator << (const T& msg)
	{
		file.OpenFile();
		file.WriteToFile(msg, mode_warning);
		file.CloseFile();
		return *this;
	}
private:
	Mode mode_warning = Warning;
}tlf_w;
