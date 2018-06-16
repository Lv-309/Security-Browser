#pragma once
#include "stdafx.h"
#include "ErrorTypes.h"

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
