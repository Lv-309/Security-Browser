#pragma once
#include <vector>
#include <memory>

class Observer;

class Subject
{
public:
	void attach(Observer *obs);
	void setBufferData(char* val, std::size_t size);
	char* getBufferData();	
	std::size_t getSize() const;

	void notify();	

private:

	std::vector<Observer*> collection;
	std::unique_ptr<char*> buffer_data;
	std::size_t lenght;
};