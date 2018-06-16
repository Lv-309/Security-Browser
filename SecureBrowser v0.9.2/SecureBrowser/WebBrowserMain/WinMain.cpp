// Defines the entry point for the application.
#include "stdafx.h"
#include "HelpTools.h"
#include "MyWebBrowser.h"
#include "ChooseCameraWindow.h"
#include "..\TraceUtility\LogInfo.h"
#include "..\TraceUtility\LogFatal.h"

using namespace ISXMyWebBrowser;
using ISXChooseCameraWindow::ChooseCameraWindow;

int APIENTRY _tWinMain(_In_		HINSTANCE h_instance,
					   _In_opt_	HINSTANCE h_prev_instance,
					   _In_		LPWSTR    lp_cmd_line,
					   _In_		INT       n_cmd_show)
{
	int n_args;
	LPWSTR* sz_args_list = CommandLineToArgvW(GetCommandLineW(), &n_args);
	if (n_args == 1)
	{
		tlf_f << AT << "No inbound link";
		FAILURE_BOX("No inbound link");
		return EXIT_SUCCESS;
	}
	else if (n_args > 2)
	{
		tlf_f << AT << "Too many input parameters";
		FAILURE_BOX("Too many input parameters");
		return EXIT_SUCCESS;
	}

	std::unique_ptr<MyWebBrowser> lpc_wnd(MyWebBrowser::CreateSafe(h_instance, sz_args_list[1]));
	if (lpc_wnd == nullptr)
		return EXIT_SUCCESS;
	// Verify how much monitors are connected
	// TODO: Check the option when the monitor is connected after the browser starts
	if (lpc_wnd->Verify() != ErrorTypes::IS_OK)
		return EXIT_SUCCESS;
	// if all is OK, create window
	tlf_i << AT << "All stages of the verifying are successfully passed, creating a browser window";
	lpc_wnd->Create();
	

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		lpc_wnd->UpdateLink();
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}