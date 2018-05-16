#ifndef  LOGTRACE_H
#define LOGTRACE_H

#include "Log.h"

class LogTrace : public Log
{
public:
	LogTrace()
	{
		severity_level = Trace;
	}
}tlf_t;

#endif