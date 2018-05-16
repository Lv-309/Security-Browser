#ifndef  LOGDEBUG_H
#define LOGDEBUG_H

#include "Log.h"

class LogDebug : public Log
{
public:
	LogDebug()
	{
		severity_level = Debug;
	}
}tlf_d;

#endif