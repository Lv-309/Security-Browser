#include "stdafx.h"

#include "HelpTools.h"
#include "HandlingSession.h"

#include "..\TraceUtility\LogInfo.h"
#include "..\TraceUtility\LogFatal.h"

using namespace ISXHandle;

HandlingSession::HandlingSession()
{ }

ErrorTypes HandlingSession::Remote() const noexcept
{
	if (GetSystemMetrics(SM_REMOTESESSION))
		return ErrorTypes::REMOTE_SESSION;
	return ErrorTypes::IS_OK;
}

ErrorTypes HandlingSession::Virtual() const noexcept
{
	if (CreateFile(L"\\\\.\\VBoxMiniRdrDN", GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr) != INVALID_HANDLE_VALUE)
		return ErrorTypes::VIRTUAL_SESSION;
	return ErrorTypes::IS_OK;
}

ErrorTypes HandlingSession::Verify() const noexcept
{
	if (this->Remote() == ErrorTypes::REMOTE_SESSION)
	{
		tlf_f << AT << ERROR_REMOTE_SESSION_L;
		FAILURE_BOX(ERROR_REMOTE_SESSION_B);
		return ErrorTypes::REMOTE_SESSION;
	}
	else if (this->Virtual() == ErrorTypes::VIRTUAL_SESSION)
	{
		tlf_f << AT << ERROR_VIRTUAL_SESSION_L;
		FAILURE_BOX(ERROR_VIRTUAL_SESSION_B);
		return ErrorTypes::VIRTUAL_SESSION;
	}
	tlf_i << AT << INFO_SUCCES_SESSION;
	return ErrorTypes::IS_OK;
}