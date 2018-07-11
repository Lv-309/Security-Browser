#pragma once

#include "WndCtrls.h"
#include "ErrorTypes.h"
#include "KeySequence.h"
#include "WebBrowserWindow.h"
#include "HandlingMonitors.h"
#include "RegisteringWindow.h"
#include "AuthenticationWindow.h"
#include "OleClientSideBrowserGUI.h"

#include "..\WebCamera\FaceDetector.h"

namespace ISXWebBrowserWindow
{

	namespace Registration	 = ISXRegisteringWindow;
	namespace BrowserGUI	 = ISXOleClientSideBrowserGUI;
	namespace Handle		 = ISXHandle;
	namespace Authentication = ISXAuthenticationWindow;
	namespace FD             = ISXFaceDetector;
	namespace KS			 = ISXKeySequence;


	class WebBrowserWindow : public Registration::RegisteringWindow
	{
	// Ctors description
	public:
		explicit WebBrowserWindow(HINSTANCE h_instance, LPCTSTR lpsz_link);
		~WebBrowserWindow();
	// Disabling undesirable constructions
	private:
		WebBrowserWindow(const WebBrowserWindow& other) = delete;
		WebBrowserWindow(const WebBrowserWindow&& move) = delete;
		WebBrowserWindow& operator=(const WebBrowserWindow& other) = delete;
	// Functionality
	public:
		VOID UpdateLink()     const;
		ErrorTypes InitHook() const noexcept;
		ErrorTypes GetErrorsState()   const noexcept override;
		ErrorTypes GetMonitorsState() const noexcept;
		VOID SetFont(const HWND handle, LPCTSTR font_name, SHORT font_size, LONG font_weight) const noexcept;
		static WebBrowserWindow* CreateSafe(HINSTANCE h_instance, LPCTSTR lpsz_link);
	// Stages
	private:
		ErrorTypes StartTestPassing(HWND hwnd, const RECT& rc_client)    noexcept;
		ErrorTypes StartAuthentication(HWND hwnd, const RECT& rc_client) noexcept;
	// Create controls
	private: 
		HWND CreateLabel(LPCTSTR lpsz_text, const RECT& rc, HWND hwnd)  const noexcept;
		HWND CreateButton(LPCTSTR lpsz_name, const RECT& rc, HWND hwnd) const noexcept;
		HWND CreateButton(LPCTSTR lpsz_name, HWND hwnd, const RECT& rc, HMENU menu) const noexcept;
		HWND CreateEditBox(LPCTSTR lpsz_lnk, const RECT& rc, HWND hwnd) const noexcept;
	// Callback methods
	protected:
		LRESULT CALLBACK WndProc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param) override;
		static LRESULT FAR PASCAL GetMsgHookProc(int nCode, WPARAM wParam, LPARAM lParam);
	// Private fields
	private:
		HWND								 m_hwnd_address_bar = nullptr;
		LPCTSTR								 m_lpsz_link;
		// Our classes
		Handle::HandlingMonitors			 m_handle_monitors;
		Authentication::AuthenticationWindow m_auth_window;
		FD::FaceDetector*		  			 m_lpface_detector;
		std::unique_ptr<KS::KeySequence>	 m_lpkey_seq;
		// Thread for camera
		std::future<void>					 m_async_cam_thread;
	// Static fields
	private:
		static HHOOK						                sm_msg_hook;
		static CComPtr<BrowserGUI::OleClientSideBrowserGUI>	sm_lpwb_wnd;
	};

}