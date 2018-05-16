#pragma once
#include "MyWindow.h"
#include "VerifyLink.h"
#include "ErrorTypes.h"
#include "HandleSession.h"
#include "HandleMonitors.h"
#include "WebBrowserWindow.h"

using ISXMyWindow::MyWindow;
using ISXVerifyLink::VerifyLink;
using ISXErrorTypes::ErrorTypes;
using ISXHandleSession::HandleSession;
using ISXHandleMonitors::HandleMonitors;
using ISXWebBrowserWindow::WebBrowserWindow;

namespace ISXMyWebBrowser
{
	#define BTN_REFRESH 1

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
		ErrorTypes Verify() const;
		VOID CreateButton(LPCTSTR lpsz_name, HWND hwnd, HINSTANCE h_inst) const;
		HWND CreateEditBox(LPCTSTR lpsz_lnk, HWND hwnd, HINSTANCE h_inst) const;
		LRESULT CALLBACK WndProc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param) override;
	// Disable undesirable methods
	private:
		MyWebBrowser()							     = delete;
		MyWebBrowser(const MyWebBrowser&)			 = delete;
		MyWebBrowser& operator=(const MyWebBrowser&) = delete;
	// Factory
	public:
		static MyWebBrowser* CreateSafe(HINSTANCE h_instance, LPCTSTR lpsz_link);
	// Private fields
	private:	
		HWND						m_hwnd_address_bar;
		LPCTSTR						m_lpsz_link;
		// Our classes
		HandleMonitors				m_handle_monitors;
		CComPtr<WebBrowserWindow>	m_lpwb_wnd;
	};
}