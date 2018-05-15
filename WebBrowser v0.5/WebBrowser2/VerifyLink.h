#pragma once
#include "stdafx.h"
#include "HelpTools.h"
namespace ISXVerifyLink
{
	class VerifyLink
	{
	public:
		typedef std::wstring TSTR;
	// Ctors
	public:
		VerifyLink(LPCTSTR lpsz_link);
		~VerifyLink() = default;
	// Getters
	public:
		TSTR GetProtocol()   const noexcept;
		TSTR GetDomain()     const noexcept;
	// Validity check
	protected:
		BOOL CheckProtocol() const noexcept;
		BOOL CheckDomain()   const noexcept;
	// Protocol = https, domain = softserve.academy
	private:
		TSTR m_getted_url;
		TSTR m_protocol;
		TSTR m_domain;
	};

}
