// Defines the entry point for the application.
#include "stdafx.h"
#include "WebBrowser.h"

using namespace ISXMyWebBrowser;

int APIENTRY _tWinMain(_In_		HINSTANCE h_instance,
					   _In_opt_	HINSTANCE h_prev_instance,
					   _In_		LPWSTR    lp_cmd_line,
					   _In_		int       n_cmd_show)
{
	MyWindow* lpc_wnd = new MyWebBrowser(TEXT("WebBrowser"), h_instance, TEXT("WebBrowserWindow"));
	lpc_wnd->Create();
	lpc_wnd->Show();

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
