#pragma once
#include "stdafx.h"
#include "ErrorTypes.h"

namespace ISXHandle
{
	class HandlingSession
	{
	// Ctors
	public:
		explicit HandlingSession();
	// Controls
	private:
		ErrorTypes Remote()  const noexcept;
		ErrorTypes Virtual() const noexcept;
	// Verify session
	public:
		ErrorTypes Verify()  const noexcept;
	// Dctors
	public:
		~HandlingSession() = default;
	};

}
