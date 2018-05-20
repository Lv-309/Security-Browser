#pragma once
#include "Log.h"
#include "Severity.h"

class LogWarning : public Log
{
public:
	LogWarning()
	{
		severity_level = Severity::Warning;
	}
}tlf_w;