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
	// Controls
	private:
		ErrorTypes Remote()  const noexcept;
		ErrorTypes Virtual() const noexcept;
	// Verify session
	public:
		ErrorTypes Verify()  const noexcept;
	// Dctors
	public:
		~HandleSession() = default;
	};

}
