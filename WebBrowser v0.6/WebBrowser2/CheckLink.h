#pragma once
#include "stdafx.h"
#include "HelpTools.h"
namespace ISXCheckLink
{
	class CheckLink
	{
	public:
		typedef std::wstring TSTR;
		CheckLink(LPCTSTR lpsz_link);
		TSTR GetProtocol()   const;
		TSTR GetDomain()     const;
		BOOL CheckProtocol() const;
		BOOL CheckDomain()   const;
	private:
		TSTR m_getted_url;
		TSTR m_protocol;
		TSTR m_domain;
	};

}
