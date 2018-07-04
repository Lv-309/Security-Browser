#include "Observer.h"
#include "Subject.h"

	Observer::Observer(Subject* elem)
	{
		element = elem;

		element->attach(this);
	}

	Subject* Observer::getSubject()
	{
		return element;
	}
