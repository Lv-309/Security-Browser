#ifndef LOGERROR_H
#define LOGERROR_H

#include "Log.h"

class LogError : public Log
{
public:
	LogError()
	{
		severity_level = Error;
	}
}tlf_e;

#endif