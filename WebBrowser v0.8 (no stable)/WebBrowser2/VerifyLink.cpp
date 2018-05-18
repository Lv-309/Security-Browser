#include "stdafx.h"
#include "VerifyLink.h"
using namespace ISXVerifyLink;

VerifyLink::VerifyLink(LPCTSTR lpsz_link)
	: m_getted_url(lpsz_link)
{
	std::wregex  url_regex(
			L"(http|https)://([^/ :]+):?([^/ ]*)(/?[^ #?]*)\\x3f?([^ #]*)#?([^ ]*)"
		);
	std::wsmatch result;
	if (regex_match(this->m_getted_url, result, url_regex))
	{
		this->m_protocol = std::wstring(result[1].first, result[1].second);
		this->m_domain   = std::wstring(result[2].first, result[2].second);
	}
	else
	{
		// TODO: Throw error, it's not link
		FAILURE_BOX("It must be a link!");
	}
}

wstring VerifyLink::GetProtocol() const noexcept
{
	return this->m_protocol;
}

wstring VerifyLink::GetDomain() const noexcept
{
	return this->m_domain;
}

ErrorTypes VerifyLink::Verify() const noexcept
{
	if (GetProtocol().compare(L"http")  != 0 &&
		GetProtocol().compare(L"https") != 0)
	{
		// TODO: Throw error link without https
		FAILURE_BOX("Incorrect link no http or https");
		return ErrorTypes::INCORRECTLINK;
	}
	else if (GetDomain().compare(L"softserve.academy") != 0)
	{
		// TODO: Throw error incorrect link
		FAILURE_BOX("Incorrect link");
		return ErrorTypes::INCORRECTLINK;
	}
	return ErrorTypes::ISOK;
}