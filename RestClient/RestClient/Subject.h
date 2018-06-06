#pragma once
#include <vector>
#include <memory>

class Observer;

class Subject
{
public:
	void attach(Observer *obs);
	void setBufferData(char* val);
	char* getBufferData();	

	void notify();	

private:

	std::vector<Observer*> collection;
	std::unique_ptr<char*> buffer_data;	
};