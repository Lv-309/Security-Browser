#include "Subject.h"

class Observer
{
private:

	Subject* element;

public:

	Observer(Subject* elem);	

	virtual void Request(void) = 0;	
	virtual void Request(std::string) = 0;

protected:

	Subject* GetSubject();
	
};