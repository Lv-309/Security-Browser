#pragma once

#include "stdafx.h"
#include "ErrorTypes.h"

using std::wstring;

namespace ISXVerifyLink
{
	class VerifyLink
	{
	// Ctors
	public:
		 VerifyLink(LPCTSTR lpsz_link = L"https://softserve.academy/");
		~VerifyLink() = default;
	// Controls
	public:
		wstring GetProtocol()   const noexcept;
		wstring GetDomain()     const noexcept;
		// Validity check
		ErrorTypes Verify() const noexcept;
	// Protocol = https, domain = softserve.academy
	private:
		wstring m_domain = L"softserve.academy";
		wstring m_protocol = L"https";
		wstring m_getted_url = L"https://softserve.academy/";
	};

}
