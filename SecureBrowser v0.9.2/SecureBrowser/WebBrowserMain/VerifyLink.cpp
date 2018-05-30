#include "stdafx.h"
#include "HelpTools.h"
#include "VerifyLink.h"
#include "..\TraceUtility\LogInfo.h"
#include "..\TraceUtility\LogFatal.h"
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
		tlf_f << AT << "Incorrect input parameter";
		FAILURE_BOX("The input parameter must be a link");
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
		tlf_f << AT << "Incorrect link no http or https";
		FAILURE_BOX("Incorrect link no http or https");
		return ErrorTypes::INCORRECT_LINK;
	}
	else if (GetDomain().compare(L"softserve.academy") != 0)
	{
		tlf_f << AT << "Incorrect link";
		FAILURE_BOX("Incorrect link");
		return ErrorTypes::INCORRECT_LINK;
	}
	tlf_i << AT << "Successful input link verification";
	return ErrorTypes::IS_OK;
}