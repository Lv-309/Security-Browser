#pragma once
#include "MyWindow.h"
#include "CheckLink.h"
#include "WebBrowserWindow.h"

using ISXMyWindow::MyWindow;
using ISXCheckLink::CheckLink;
using ISXWebBrowserWindow::WebBrowserWindow;

namespace ISXMyWebBrowser
{
	#define BTN_REFRESH 1
	#define BTN_GO		2

	class MyWebBrowser : public MyWindow
	{
	// Ctors definition
	public:
		explicit MyWebBrowser(HINSTANCE h_instance, LPCTSTR lpsz_link);
		explicit MyWebBrowser(LPCTSTR lpsz_caption, LPWNDCLASSEX lpwn_class_ex, LPCTSTR lpsz_link);
	#ifdef _INIT_CUSTOM_WND
		explicit MyWebBrowser(LPCTSTR lpsz_caption, HINSTANCE h_instance, LPCTSTR lpsz_class_name,
				UINT style, HICON h_icon, HCURSOR h_cursor, HBRUSH hbr_background,
				LPCTSTR lpsz_menu_name, HICON h_icon_sm);
	#endif
		~MyWebBrowser();
	// MyWebBrowser controls
	public:
		VOID UpdateWindow() const;
		VOID CreateButton(LPCTSTR lpsz_name, HWND hwnd, HINSTANCE h_inst) const;
		HWND CreateEditBox(LPCTSTR lpsz_lnk, HWND hwnd, HINSTANCE h_inst) const;
		LRESULT CALLBACK WndProc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param) override;
	// Disable undesirable methods
	private:
		MyWebBrowser()							     = delete;
		MyWebBrowser(const MyWebBrowser&)			 = delete;
		MyWebBrowser& operator=(const MyWebBrowser&) = delete;
	// Getting as aggregated WebBrowserWindow class
	private:
		CheckLink			m_chk_link;
		WebBrowserWindow*	m_lpwb_wnd;
		HWND				m_hwnd_address_bar;
		LPCTSTR				m_lpsz_link;
	};
}