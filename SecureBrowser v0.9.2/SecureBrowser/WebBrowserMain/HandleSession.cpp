#include "stdafx.h"
#include "HelpTools.h"
#include "HandleSession.h"
#include "..\TraceUtility\LogInfo.h"
#include "..\TraceUtility\LogFatal.h"
using namespace ISXHandleSession;

HandleSession::HandleSession()
{ }

bool HandleSession::Remote() const
{
	if (GetSystemMetrics(SM_REMOTESESSION))
		return true;
	return false;
}

bool HandleSession::Virtual() const
{
	return false;
	if (CreateFile(L"\\\\.\\VBoxMiniRdrDN", GENERIC_READ, FILE_SHARE_READ,
		nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr) != INVALID_HANDLE_VALUE)
	{
		return true;
	}

	return false;
}

ErrorTypes HandleSession::Verify() const
{
	if (this->Remote() == true)
	{
		tlf_f << AT << "User try to pass the test in a remote session";
		FAILURE_BOX("You cannot pass the test in a remote session");
		return ErrorTypes::REMOTE_SESSION;
	}
	else if (this->Virtual() == true)
	{
		tlf_f << AT << "User try to pass the test in a virtual session";
		FAILURE_BOX("You cannot pass the test in a virtual session");
		return ErrorTypes::VIRTUAL_SESSION;
	}
	tlf_i << AT << "Successful user session verification";
	return ErrorTypes::IS_OK;
}