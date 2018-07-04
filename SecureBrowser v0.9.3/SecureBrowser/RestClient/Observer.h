#include "Subject.h"

class Observer
{
private:

	Subject* element;

public:

	Observer(Subject* elem);	

	virtual void request() = 0;	

protected:

	Subject* getSubject();
	
};