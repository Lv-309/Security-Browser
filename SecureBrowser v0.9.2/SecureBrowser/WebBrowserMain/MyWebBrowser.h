#pragma once
#include "MyWindow.h"
#include "ErrorTypes.h"
#include "HandleMonitors.h"
#include "WebBrowserWindow.h"
#include "ChooseCameraWindow.h"
#include "..\WebCamera\Frame.h"
#include "..\WebCamera\FaceDetector.h"
#include "IdWindow.h"

using ISXMyWindow::MyWindow;
using ISXHandleMonitors::HandleMonitors;
using ISXWebBrowserWindow::WebBrowserWindow;
using ISXChooseCameraWindow::ChooseCameraWindow;



enum class WndControls : unsigned short int
{
	ID_WEBCAM_CHILD,
	ID_GO_BTN,
	ID_LOGO, // potomkam
	ID_EDIT_BOX,
	ID_VERIFY_CHILD,
	ID_SUBMIT_BTN,
	ID_RESET_BTN,
	ID_PHOTO_BTN,
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
		BOOL InitDialog();
		VOID UpdateLink() const;
		ErrorTypes Verify() const noexcept;
		VOID SetFont(const HWND handle, LPCTSTR font_name, SHORT font_size, LONG font_weight) const noexcept;
		static MyWebBrowser* CreateSafe(HINSTANCE h_instance, LPCTSTR lpsz_link);
	// Stages
	private:
		ErrorTypes TestPassing(HWND hwnd, const RECT& rc_client)    noexcept;
		ErrorTypes Authentication(HWND hwnd, const RECT& rc_client) noexcept;
	// Create controls
	private: 
		void IdCreation(HWND hparent);
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
		//RECT								m_rc_client;
		ISXFaceDetector::FaceDetector		fd;
		std::thread							t;
		HWND								m_hwnd_address_bar = nullptr;
		LPCTSTR								m_lpsz_link;
		// Our classes
		ChooseCameraWindow					m_cc_wnd;
		HandleMonitors						m_handle_monitors;
		CComPtr<WebBrowserWindow>			m_lpwb_wnd;
		static HHOOK						sm_msg_hook;
		static CComPtr<WebBrowserWindow>	sm_lpwb_wnd;
		ISXIdWindow::IdWindow				id;
	};

}