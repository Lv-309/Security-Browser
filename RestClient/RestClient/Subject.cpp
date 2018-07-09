#include "Subject.h"
#include "Observer.h"

	void Subject::Attach(Observer *obs)
	{
		collection.push_back(obs);
	}

	void Subject::SetBufferData(char* val, size_t size, std::wstring name_file)
	{
		lenght = size;	
		file_name = name_file;
		buffer_data = std::make_unique<char[]>(size + 1);

		for (size_t i = 0; i != size; i++)
			buffer_data[i] = val[i];
		buffer_data[size] = '\0';

		Notify();
	}

	size_t Subject::GetSize() const
	{
		return lenght;
	}

	std::wstring Subject::GetFileName() const
	{
		return file_name;
	}

	char* Subject::GetBufferData()
	{
		if (buffer_data)
			return buffer_data.get();
		else
			return nullptr;
	}

	void Subject::Notify()
	{
		for (int i = 0; i < collection.size(); i++)
					collection[i]->Request();
	}