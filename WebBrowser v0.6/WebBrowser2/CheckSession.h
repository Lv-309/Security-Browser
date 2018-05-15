#pragma once
#include "stdafx.h"
#include "HelpTools.h"
namespace ISXCheckSession
{
	class CheckSession
	{
	public:
		CheckSession();
		BOOL CheckRemoteSession();
		BOOL CheckVirtualSession();
	};

}	