#pragma once
#include <vector>
#include <memory>

class Observer;

class Subject
{
public:
	void attach(Observer *obs);
	void setBufferData(char* val, size_t size, std::wstring file_name);
	char* getBufferData();	
	size_t getSize() const;
	std::wstring getFileName() const;
	void notify();	

private:
	std::vector<Observer*> collection;
	std::unique_ptr<char*> buffer_data;
	std::size_t lenght;
	std::wstring file_name;
};