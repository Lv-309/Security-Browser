#include "stdafx.h"
#include "HandleSession.h"
using namespace ISXHandleSession;

HandleSession::HandleSession()
{ }

bool HandleSession::Remote() const
{
	//return true;		// to miss checking
	if (GetSystemMetrics(SM_REMOTESESSION))
	{
		FAILURE_BOX("You cannot pass the test in a remote session");
		return true;
	}
	return false;
}

bool HandleSession::Virtual() const
{
	//return 0;
	bool rc = true;
	__try
	{
		__asm
		{
			push   edx

			push   ecx
			push   ebx

			mov    eax, 'VMXh'
			mov    ebx, 0		// any value but not the MAGIC VALUE
			mov    ecx, 10		// get VMWare version
			mov    edx, 'VX'	// port number

			in     eax, dx		// read port
								// on return EAX returns the VERSION
			cmp    ebx, 'VMXh'	// is it a reply from VMWare?
			setz[rc]			// set return value

			pop    ebx
			pop    ecx
			pop    edx
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		rc = false;
	}

	if (rc == true)
	{
		FAILURE_BOX("You cannot pass the test in a virtual session");
		return rc;
	}
	return rc;
}

ErrorTypes HandleSession::Verify() const
{
	if (this->Remote() == true)
		return ErrorTypes::REMOTESESSION;
	else if (this->Virtual() == true)
		return ErrorTypes::VIRTUALSESSION;

	return ErrorTypes::ISOK;
}