#pragma once
#include "MyWindow.h"
#include "ErrorTypes.h"
#include "KeySequence.h"
#include "HandleMonitors.h"
#include "WebBrowserWindow.h"
#include "ChooseCameraWindow.h"

using ISXMyWindow::MyWindow;
using ISXHandleMonitors::HandleMonitors;
using ISXWebBrowserWindow::WebBrowserWindow;
using ISXChooseCameraWindow::ChooseCameraWindow;

enum class WndControls : unsigned short int
{
	ID_GO_BTN,
	ID_LOGO, // potomkam
	ID_EDIT_BOX,
};

namespace ISXMyWebBrowser
{

	class MyWebBrowser : public MyWindow
	{
		// Ctors description
	public:
		explicit MyWebBrowser(HINSTANCE h_instance, LPCTSTR lpsz_link);
		~MyWebBrowser();
		// Disabling undesirable constructions
	private:
		MyWebBrowser(const MyWebBrowser& other) = delete;
		MyWebBrowser(const MyWebBrowser&& move) = delete;
		MyWebBrowser& operator=(const MyWebBrowser& other) = delete;
		// Functionality
	public:
		VOID UpdateLink() const;
		ErrorTypes Verify() const noexcept;
		static MyWebBrowser* CreateSafe(HINSTANCE h_instance, LPCTSTR lpsz_link);
	// Self methods
	private:
		ErrorTypes InitHook() const noexcept;
		ErrorTypes TestPassing(HWND hwnd, const RECT& rc_client)    noexcept;
		ErrorTypes Authentication(HWND hwnd, const RECT& rc_client) noexcept;
	// Create controls
	protected:
		HWND CreateLabel(LPCTSTR lpsz_text, const RECT& rc, HWND hwnd)  const noexcept;
		HWND CreateButton(LPCTSTR lpsz_name, const RECT& rc, HWND hwnd) const noexcept;
		HWND CreateButton(LPCTSTR lpsz_name, const RECT& rc, HMENU menu, HWND hwnd) const noexcept;
		HWND CreateEditBox(LPCTSTR lpsz_lnk, const RECT& rc, HWND hwnd) const noexcept;
	// Callback methods
	protected:
		LRESULT CALLBACK WndProc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param) override;
		static LRESULT CALLBACK GetMsgHookProc(int n_code, WPARAM w_param, LPARAM l_param);
	// Private fields
	private:
		HWND						m_hwnd_address_bar = nullptr;
		LPCTSTR						m_lpsz_link;
		// Our classes
		ChooseCameraWindow			m_cc_wnd;
		HandleMonitors				m_handle_monitors;

		std::unique_ptr<ISXKeySequence::KeySequence> m_lpkey_seq;

		static HHOOK					 sm_msg_hook;
		static CComPtr<WebBrowserWindow> sm_lpwb_wnd;
	};
}