#ifndef  LOGWARNING_H
#define LOGWARNING_H

#include "Log.h"

class LogWarning : public Log
{
public:
	LogWarning()
	{
		severity_level = Warning;
	}
}tlf_w;

#endif