#pragma once
#include "Log.h"
#include "Severity.h"

class LogDebug : public Log
{
public:
	LogDebug()
	{
		severity_level = Severity::Debug;
	}
}tlf_d;