#include "stdafx.h"
#include "CheckSession.h"
using namespace ISXCheckSession;

CheckSession::CheckSession()
{
	CheckRemoteSession();
	CheckVirtualSession();
}
BOOL CheckSession::CheckRemoteSession()
{
	return true;		// to miss checking
	bool isRemotable = false;
	if (GetSystemMetrics(SM_REMOTESESSION))
	{
		isRemotable = true;
		FAILURE_BOX("You cannot pass the test in a remote session");
		exit(EXIT_SUCCESS);
	}
	return isRemotable;
}

BOOL CheckSession::CheckVirtualSession()
{
	return 0;
	bool rc = true;

	__try
	{
		__asm
		{
			push   edx

			push   ecx
			push   ebx

			mov    eax, 'VMXh'
			mov    ebx, 0 // any value but not the MAGIC VALUE
			mov    ecx, 10 // get VMWare version
			mov    edx, 'VX' // port number

			in     eax, dx // read port
						   // on return EAX returns the VERSION
						   cmp    ebx, 'VMXh' // is it a reply from VMWare?
						   setz[rc] // set return value

						   pop    ebx
						   pop    ecx
						   pop    edx
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		rc = false;
	}
	if (rc) {
		FAILURE_BOX("You cannot pass the test in a virtual session");
		exit(EXIT_SUCCESS);
	}
	return rc;

}