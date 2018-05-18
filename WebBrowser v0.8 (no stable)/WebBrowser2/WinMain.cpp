// Defines the entry point for the application.
#include "stdafx.h"
#include "HelpTools.h"
#include "MyWebBrowser.h"

using namespace ISXMyWebBrowser;
using std::unique_ptr;

int APIENTRY _tWinMain(_In_		HINSTANCE h_instance,
					   _In_opt_	HINSTANCE h_prev_instance,
					   _In_		LPWSTR    lp_cmd_line,
					   _In_		int       n_cmd_show)
{
	int n_args;
	LPWSTR* sz_args_list = CommandLineToArgvW(GetCommandLineW(), &n_args);
	if (n_args == 1)
	{
		FAILURE_BOX("No link");
		return EXIT_SUCCESS;
	}
	else if (n_args > 2)
	{
		FAILURE_BOX("Too much args");
		return EXIT_SUCCESS;
	}

	unique_ptr<MyWebBrowser> lpc_wnd(MyWebBrowser::CreateSafe(h_instance, sz_args_list[1]));
	if (lpc_wnd == nullptr)
		return EXIT_SUCCESS;
	// Verify how much monitors are connected
	// TODO: Check the option when the monitor is connected after the browser starts
	if (lpc_wnd->Verify() != ErrorTypes::ISOK)
		return EXIT_SUCCESS;
	// if all is OK, create window
	lpc_wnd->Create();

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		//lpc_wnd->UpdateWindow();
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
