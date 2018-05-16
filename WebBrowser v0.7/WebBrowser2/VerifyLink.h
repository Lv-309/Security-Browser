#pragma once

#include "stdafx.h"
#include "HelpTools.h"
#include "ErrorTypes.h"

using std::wstring;
using ISXErrorTypes::ErrorTypes;

namespace ISXVerifyLink
{
	class VerifyLink
	{
	// Ctors
	public:
		 VerifyLink(LPCTSTR lpsz_link);
		~VerifyLink() = default;
	// Controls
	public:
		wstring GetProtocol()   const noexcept;
		wstring GetDomain()     const noexcept;
		// Validity check
		ErrorTypes Verify() const noexcept;
	// Protocol = https, domain = softserve.academy
	private:
		wstring m_domain;
		wstring m_protocol;
		wstring m_getted_url;
	};

}
