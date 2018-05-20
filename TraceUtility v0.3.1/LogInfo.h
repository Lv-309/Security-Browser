#pragma once
#include "Log.h" 
#include "Severity.h"

class LogInfo : public Log
{
public:
	LogInfo()
	{
		severity_level = Severity::Info;
	}
}tlf_i;