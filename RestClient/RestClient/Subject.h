#pragma once
#include <vector>
#include <memory>

class Observer;

class Subject
{
public:
	void Attach(Observer *obs);
	void SetBufferData(char* val, size_t size, std::wstring file_name);
	char* GetBufferData();	
	size_t GetSize() const;
	std::wstring GetFileName() const;
	void Notify();	

private:
	std::vector<Observer*> collection;
	std::unique_ptr<char[]> buffer_data;
	std::size_t lenght;
	std::wstring file_name;
};