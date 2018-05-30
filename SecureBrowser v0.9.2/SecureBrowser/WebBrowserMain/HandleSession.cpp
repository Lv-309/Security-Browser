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
	//return true;		// to miss checking
	if (GetSystemMetrics(SM_REMOTESESSION))
		return true;
	return false;
}

bool HandleSession::Virtual() const
{
	bool rc = true;
	//__try
	//{
	//	__asm
	//	{
	//		push   edx

	//		push   ecx
	//		push   ebx

	//		mov    eax, 'VMXh'
	//		mov    ebx, 0		// any value but not the MAGIC VALUE
	//		mov    ecx, 10		// get VMWare version
	//		mov    edx, 'VX'	// port number

	//		in     eax, dx		// read port
	//							// on return EAX returns the VERSION
	//		cmp    ebx, 'VMXh'	// is it a reply from VMWare?
	//		setz[rc]			// set return value

	//		pop    ebx
	//		pop    ecx
	//		pop    edx
	//	}
	//}
	//__except (EXCEPTION_EXECUTE_HANDLER)
	//{
	//	rc = false;
	//}
	rc = false;
	return rc;
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