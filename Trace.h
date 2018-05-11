#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <ctime>

enum Mode {Standard, Error, Warning};

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
		if (mode == Standard)
		{
			ofile << "Default\t" << msg << "\t";

			//ofile << __FILE__ << "\t" << __FUNCTION__ << "\t" << __LINE__ << "\t";

			std::time_t result = std::time(nullptr);
			ofile << "Date: " << std::asctime(std::localtime(&result));
		}
		else if (mode == Error)
		{
			ofile << "Error\t" << msg << "\t";
			std::time_t result2 = std::time(nullptr);
			ofile << "Date: " << std::asctime(std::localtime(&result2));
		}
		else if (mode == Warning)
		{
			ofile << "Warning\t" << msg << "\t";
			std::time_t result3 = std::time(nullptr);
			ofile << "Date: " << std::asctime(std::localtime(&result3));
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