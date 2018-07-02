#include "stdafx.h"
#include "HelpTools.h"
#include "VerifyLink.h"
#include "..\TraceUtility\LogInfo.h"
#include "..\TraceUtility\LogFatal.h"
using namespace ISXVerifyLink;

VerifyLink::VerifyLink(LPCTSTR lpsz_link)
	: m_getted_url(lpsz_link)
{
	std::wregex  url_regex(L"(http|https)://([^/ :]+):?([^/ ]*)(/?[^ #?]*)\\x3f?([^ #]*)#?([^ ]*)");
	std::wsmatch result;
	if (regex_match(this->m_getted_url, result, url_regex))
	{
		this->m_protocol = std::wstring(result[1].first, result[1].second);
		this->m_domain   = std::wstring(result[2].first, result[2].second);
	}
	else
	{
		tlf_f << AT << ERROR_WRONG_PARAM;
		FAILURE_BOX(ERROR_WRONG_PARAM);
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
	return ErrorTypes::IS_OK;
	if (GetProtocol().compare(HTTP)  != 0 ||
		GetProtocol().compare(HTTPS) != 0)
	{
		tlf_f << AT << ERROR_WRONG_LINK;
		FAILURE_BOX(ERROR_WRONG_LINK);
		return ErrorTypes::INCORRECT_LINK;
	}
	else if (GetDomain().compare(SOFTSERVE_LINK) != 0 ||
			 GetDomain().compare(TEST_LINK)      != 0)
	{
		tlf_f << AT << ERROR_WRONG_LINK;
		FAILURE_BOX(ERROR_WRONG_LINK);
		return ErrorTypes::INCORRECT_LINK;
	}
	tlf_i << AT << INFO_CORRECT_LINK;
	return ErrorTypes::IS_OK;
}