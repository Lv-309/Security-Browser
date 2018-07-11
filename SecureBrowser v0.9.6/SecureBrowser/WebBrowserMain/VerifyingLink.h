#pragma once

#include "stdafx.h"
#include "ErrorTypes.h"

namespace ISXVerifyingLink
{

	class VerifyingLink
	{
	// Ctors
	public:
		 VerifyingLink(LPCTSTR lpsz_link);
		~VerifyingLink() = default;
	// Controls
	public:
		std::wstring GetDomain()   const noexcept;
		std::wstring GetProtocol() const noexcept;
		// Validity check
		ErrorTypes Verify() const noexcept;
	// Protocol = https, domain = softserve.academy
	private:
		std::wstring m_domain;
		std::wstring m_protocol;
		std::wstring m_getted_url;
	};

}
