#include "Subject.h"
#include "Observer.h"

	void Subject::attach(Observer *obs)
	{
		collection.push_back(obs);
	}

	void Subject::setBufferData(char* val, size_t size, std::wstring name_file)
	{
		lenght = size;	
		file_name = name_file;
		buffer_data = std::make_unique<char*>(val);
		notify();
	}

	size_t Subject::getSize() const
	{
		return lenght;
	}

	std::wstring Subject::getFileName() const
	{
		return file_name;
	}

	char* Subject::getBufferData()
	{
		if (buffer_data)
			return *buffer_data;
		else
			return nullptr;
	}

	void Subject::notify()
	{
		for (int i = 0; i < collection.size(); i++)
					collection[i]->request();
	}