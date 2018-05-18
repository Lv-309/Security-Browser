#pragma once

#include "stdafx.h"
#include "HelpTools.h"
#include "ErrorTypes.h"

using ISXErrorTypes::ErrorTypes;

namespace ISXHandleSession
{
	class HandleSession
	{
	// Ctors
	public:
		explicit HandleSession();
		~HandleSession() = default;
	// Controls
	private:
		bool Remote()  const;
		bool Virtual() const;
	// Verify session
	public:
		ErrorTypes Verify() const;
	};

}
