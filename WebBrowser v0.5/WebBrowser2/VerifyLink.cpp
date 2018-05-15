#include "stdafx.h"
#include "VerifyLink.h"
using namespace ISXVerifyLink;

VerifyLink::VerifyLink(LPCTSTR lpsz_link)
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

		if (this->CheckProtocol() == FALSE) exit(EXIT_SUCCESS);
		if (this->CheckDomain() == FALSE)   exit(EXIT_SUCCESS);
	}
	else
	{
		// TODO: Throw error, it's not link
		FAILURE_BOX("It must be a link!");
	}
}

VerifyLink::TSTR VerifyLink::GetProtocol() const noexcept
{
	return this->m_protocol;
}

VerifyLink::TSTR VerifyLink::GetDomain() const noexcept
{
	return this->m_domain;
}

BOOL VerifyLink::CheckProtocol() const noexcept
{
	if (GetProtocol().compare(L"https") != 0)
	{
		// TODO: Throw error link without https
		FAILURE_BOX("Incorrect link no https");
		return FALSE;
	}
	return TRUE;
}

BOOL VerifyLink::CheckDomain() const noexcept
{
	if (GetDomain().compare(L"softserve.academy") != 0)
	{
		// TODO: Throw error incorrect link
		FAILURE_BOX("Incorrect link");
		return FALSE;
	}
	return TRUE;
}