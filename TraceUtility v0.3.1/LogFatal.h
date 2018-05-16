#ifndef LOGFATAL_H
#define LOGFATAL_H

#include "Log.h"

class LogFatal : public Log
{
public:
	LogFatal()
	{
		severity_level = Fatal;
	}
}tlf_f;

#endif