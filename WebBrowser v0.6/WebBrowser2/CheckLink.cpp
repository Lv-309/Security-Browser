#include "stdafx.h"
#include "CheckLink.h"
using namespace ISXCheckLink;

CheckLink::CheckLink(LPCTSTR lpsz_link)
	: m_getted_url(lpsz_link)
{
	std::wregex  url_regex(
		L"(https)://([^/ :]+):?([^/ ]*)(/?[^ #?]*)\\x3f?([^ #]*)#?([^ ]*)"
	);
	std::wsmatch result;
	if (regex_match(this->m_getted_url, result, url_regex))
	{
		this->m_protocol = std::wstring(result[1].first, result[1].second);
		this->m_domain   = std::wstring(result[2].first, result[2].second);
	}
	else
	{
		FAILURE_BOX("It must be a link!");
	}
}

CheckLink::TSTR CheckLink::GetProtocol() const
{
	return this->m_protocol;
}

CheckLink::TSTR CheckLink::GetDomain() const
{
	return this->m_domain;
}

BOOL CheckLink::CheckProtocol() const
{
	if (GetProtocol().compare(L"https") != 0)
	{
		FAILURE_BOX("Incorrect link no https");
		return FALSE;
	}
	return TRUE;
}

BOOL CheckLink::CheckDomain() const
{
	if (GetDomain().compare(L"softserve.academy") != 0)
	{
		FAILURE_BOX("Incorrect link");
		return FALSE;
	}
	return TRUE;
}