// Defines the entry point for the application.
#include "stdafx.h"
#include "WebBrowser.h"

using namespace ISXMyWebBrowser;

int APIENTRY _tWinMain(_In_		HINSTANCE h_instance,
					   _In_opt_	HINSTANCE h_prev_instance,
					   _In_		LPWSTR    lp_cmd_line,
					   _In_		int       n_cmd_show)
{
	int n_args;
	LPWSTR* lp_args = CommandLineToArgvW(GetCommandLineW(), &n_args);
	std::wstring str_url(lp_args[1]);
	std::wsmatch wideMatch;
	std::wregex wrx(L"(http|https)://([^/ :]+):?([^/ ]*)(/?[^ #?]*)\\x3f?([^ #]*)#?([^ ]*)");

	if (regex_match(str_url.cbegin(), str_url.cend(), wideMatch, wrx))
	{
		std::wstring protocol = std::wstring(wideMatch[1].first, wideMatch[1].second);
		std::wstring domain = std::wstring(wideMatch[2].first, wideMatch[2].second);
		std::wstring port = std::wstring(wideMatch[3].first, wideMatch[3].second);
		std::wstring path = std::wstring(wideMatch[4].first, wideMatch[4].second);
		std::wstring query = std::wstring(wideMatch[5].first, wideMatch[5].second);
	}
	// Checking for right link 
	
	MyWindow* lpc_wnd = new MyWebBrowser(TEXT("WebBrowser"), h_instance, TEXT("WebBrowserWindow"));
	lpc_wnd->Setwparam(&str_url);
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
