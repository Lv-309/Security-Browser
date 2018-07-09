#include "Observer.h"
#include "Subject.h"

	Observer::Observer(Subject* elem)
	{
		element = elem;

		element->Attach(this);
	}

	Subject* Observer::GetSubject()
	{
		return element;
	}
