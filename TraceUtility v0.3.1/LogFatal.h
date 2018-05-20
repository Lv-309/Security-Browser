#pragma once
#include "Log.h"
#include "Severity.h"

class LogFatal : public Log
{
public:
	LogFatal()
	{
		severity_level = Severity::Fatal;
	}
}tlf_f;