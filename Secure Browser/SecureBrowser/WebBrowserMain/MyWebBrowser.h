#pragma once
#include "MyWindow.h"
#include "WndCtrls.h"
#include "ErrorTypes.h"
#include "KeySequence.h"
#include "HandleMonitors.h"
#include "WebBrowserWindow.h"
#include "ChooseCameraWindow.h"
#include "AuthenticationWindow.h"
#include "..\WebCamera\Frame.h"
#include "..\WebCamera\FaceDetector.h"

using ISXMyWindow::MyWindow;
using ISXHandleMonitors::HandleMonitors;
using ISXWebBrowserWindow::WebBrowserWindow;
using ISXChooseCameraWindow::ChooseCameraWindow;
using ISXFaceDetector::FaceDetector;
using ISXAuthenticationWindow::AuthenticationWindow;
using ISXKeySequence::KeySequence;

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
		VOID UpdateLink()     const;
		ErrorTypes InitHook() const noexcept;
		ErrorTypes GetErrorsState()   const noexcept override;
		ErrorTypes GetMonitorsState() const noexcept;
		VOID SetFont(const HWND handle, LPCTSTR font_name, SHORT font_size, LONG font_weight) const noexcept;
		static MyWebBrowser* CreateSafe(HINSTANCE h_instance, LPCTSTR lpsz_link);
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
		HWND								m_hwnd_address_bar = nullptr;
		LPCTSTR								m_lpsz_link;
		// Our classes
		HandleMonitors						m_handle_monitors;
		AuthenticationWindow				m_auth;
		FaceDetector*						m_lpface_detector;
		std::unique_ptr<KeySequence>		m_lpkey_seq;
		// Thread for camera
		std::future<int>					m_async_cam_thread;
	// Static fields
	private:
		static HHOOK						sm_msg_hook;
		static CComPtr<WebBrowserWindow>	sm_lpwb_wnd;
	};

}