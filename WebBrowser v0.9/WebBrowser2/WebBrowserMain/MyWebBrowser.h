#pragma once
#include "MyWindow.h"
#include "ErrorTypes.h"
#include "HandleMonitors.h"
#include "WebBrowserWindow.h"

using ISXMyWindow::MyWindow;
using ISXHandleMonitors::HandleMonitors;
using ISXWebBrowserWindow::WebBrowserWindow;

enum class WndControls : unsigned short int
{
	ID_WEBCAM_CHILD,
	ID_REFRESH_BTN,
	ID_VERIFY_CHILD,
	ID_SUBMIT_BTN,
	ID_RESET_BTN,
	ID_PHOTO_BTN,
};


namespace ISXMyWebBrowser
{

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
		VOID Update() const;
		ErrorTypes Verify() const noexcept;
		ErrorTypes Authentication(RECT rc_client, HWND hwnd) const noexcept;
		HWND CreateButton(LPCTSTR lpsz_name, HWND hwnd) const noexcept;
		HWND CreateButton(LPCTSTR lpsz_name, HWND hwnd, RECT rc, HMENU menu) const noexcept;
		HWND CreateEditBox(LPCTSTR lpsz_lnk, HWND hwnd) const noexcept;
	// Callback methods
	protected:
		LRESULT CALLBACK WndProc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param) override;
		static LRESULT CALLBACK AuthChildProc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param);
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
		HWND						m_hwnd_address_bar = nullptr;
		LPCTSTR						m_lpsz_link;
		// Our classes
		HandleMonitors				m_handle_monitors;
		CComPtr<WebBrowserWindow>	m_lpwb_wnd;
	};
}