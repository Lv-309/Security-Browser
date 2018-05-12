#pragma once
#include "MyWindow.h"
#include "WebBrowserWindow.h"

using ISXMyWindow::MyWindow;
using ISXWebBrowserWindow::WebBrowserWindow;

namespace ISXMyWebBrowser
{
#define BTN_REFRESH 1
#define BTN_GO		2
	class MyWebBrowser : public MyWindow
	{
	// Ctors definition
	public:
		explicit MyWebBrowser(LPCTSTR lpsz_caption, LPWNDCLASSEX lpwn_class_ex);
		explicit MyWebBrowser(LPCTSTR lpsz_caption, HINSTANCE h_instance, LPCTSTR lpsz_class_name, LPWSTR lpwstr_url);
	#ifdef _INIT_CUSTOM_WND
		explicit MyWebBrowser(LPCTSTR lpsz_caption, HINSTANCE h_instance, LPCTSTR lpsz_class_name,
				UINT style, HICON h_icon, HCURSOR h_cursor, HBRUSH hbr_background,
				LPCTSTR lpsz_menu_name, HICON h_icon_sm);
	#endif
		~MyWebBrowser();
	// MyWebBrowser controls
	public:
		VOID CreateButton(LPCWSTR btn_name, HWND hwnd, HINSTANCE hinst) const;
		HWND CreateEditBox(LPCWSTR link, HWND hwnd, HINSTANCE hinst)	const;
		LRESULT CALLBACK WndProc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param) override;
	// Disable undesirable methods
	private:
		MyWebBrowser()							     = delete;
		MyWebBrowser(const MyWebBrowser&)			 = delete;
		MyWebBrowser& operator=(const MyWebBrowser&) = delete;
	// Getting as aggregated WebBrowserWindow class
	private:
		LPWSTR m_lpwstr_startPage;
		WebBrowserWindow* m_lpwb_wnd;
	};
}