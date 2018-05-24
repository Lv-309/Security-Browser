#pragma once
#include "Log.h"
#include "Severity.h"

class LogError : public Log
{
public:
	LogError()
	{
		severity_level = Severity::Error;
	}
}tlf_e;