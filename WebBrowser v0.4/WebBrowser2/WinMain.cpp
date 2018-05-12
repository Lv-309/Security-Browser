// Defines the entry point for the application.
#include "stdafx.h"
#include "HelpTools.h"
#include "MyWebBrowser.h"

using namespace ISXMyWebBrowser;

using std::wstring;
using std::wregex;
using std::wsmatch;
using std::regex_match;

int APIENTRY _tWinMain(_In_		HINSTANCE h_instance,
					   _In_opt_	HINSTANCE h_prev_instance,
					   _In_		LPWSTR    lp_cmd_line,
					   _In_		int       n_cmd_show)
{
	int n_args;
	LPWSTR* sz_args_list = CommandLineToArgvW(GetCommandLineW(), &n_args);
	if (n_args > 2)
	{
		FAILURE_BOX("Too much args");
		return EXIT_SUCCESS;
	}

	MyWebBrowser* lpc_wnd = new MyWebBrowser(h_instance, sz_args_list[1]);
	lpc_wnd->Create();

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		lpc_wnd->UpdateWindow();
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
