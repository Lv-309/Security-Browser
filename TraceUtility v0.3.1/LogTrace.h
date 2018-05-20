#pragma once
#include "Log.h"
#include "Severity.h"

class LogTrace : public Log
{
public:
	LogTrace()
	{
		severity_level = Severity::Trace;
	}
}tlf_t;