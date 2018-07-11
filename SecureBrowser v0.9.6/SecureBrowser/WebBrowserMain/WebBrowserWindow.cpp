#include "stdafx.h"

#include "HelpTools.h"
#include "WebBrowserWindow.h"
#include "VerifyingLink.h"
#include "HandlingSession.h"

#include "..\TraceUtility\LogInfo.h"
#include "..\TraceUtility\LogFatal.h"

using namespace ISXWebBrowserWindow;
namespace Verifying = ISXVerifyingLink;


HHOOK										 WebBrowserWindow::sm_msg_hook = nullptr;
CComPtr<BrowserGUI::OleClientSideBrowserGUI> WebBrowserWindow::sm_lpwb_wnd = nullptr;

WebBrowserWindow::WebBrowserWindow(HINSTANCE h_instance, LPCTSTR lpsz_link)
		: RegisteringWindow(WB_WINDOW_NAME, h_instance, WB_WINDOW_CLASS_NAME)
		, m_handle_monitors()
		, m_lpsz_link(lpsz_link)
		, m_auth_window(this->m_hinstance)
{ }

WebBrowserWindow::~WebBrowserWindow()
{ 
// 	if (this->m_lpface_detector != nullptr)
// 		delete this->m_lpface_detector;
}

ErrorTypes WebBrowserWindow::InitHook() const noexcept
{
	sm_msg_hook = SetWindowsHookEx(WH_GETMESSAGE, GetMsgHookProc, NULL, GetCurrentThreadId());
	if (sm_msg_hook == nullptr)
		return ErrorTypes::SET_HOOK_ERROR;
	return ErrorTypes::IS_OK;
}
VOID WebBrowserWindow::SetFont(const HWND handle, LPCTSTR font_name, SHORT font_size, LONG font_weight) const noexcept
{
	HDC hdc = GetDC(handle);
	LOGFONT log_font = { 0 };

	log_font.lfHeight = -MulDiv(font_size, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	log_font.lfWeight = font_weight;
	_tcscpy_s(log_font.lfFaceName, font_name);

	HFONT h_font = CreateFontIndirect(&log_font);
	ReleaseDC(handle, hdc);
	SendMessage(handle, WM_SETFONT, (WPARAM)h_font, (LPARAM)MAKELONG(TRUE, 0));
}

HWND WebBrowserWindow::CreateLabel(LPCTSTR lpsz_text, const RECT& rc, HWND hwnd) const noexcept
{
	return CreateWindow(
			TEXT("STATIC"),
			lpsz_text,
			WS_CHILD | WS_VISIBLE | WS_TABSTOP,
			rc.left, rc.top, rc.right, rc.bottom,
			hwnd,
			(HMENU)WndCtrls::ID_LOGO,
			this->m_hinstance,
			NULL
		);
}

HWND WebBrowserWindow::CreateButton(LPCTSTR lpsz_name, const RECT& rc, HWND hwnd) const noexcept
{
	return this->CreateButton(
			lpsz_name,
			hwnd,
			rc,
			(HMENU)WndCtrls::ID_GO_BTN
		);
}

HWND WebBrowserWindow::CreateButton(LPCTSTR lpsz_name, HWND hwnd, const RECT& rc, HMENU menu) const noexcept
{
	return CreateWindowEx(
			0,
			TEXT("BUTTON"),
			lpsz_name,
			WS_CHILD | WS_VISIBLE,
			(INT)rc.left, (INT)rc.top,
			(INT)rc.right, (INT)rc.bottom,
			hwnd,
			menu,
			this->m_hinstance,
			NULL
		);
}

HWND WebBrowserWindow::CreateEditBox(LPCTSTR lpsz_lnk, const RECT& rc, HWND hwnd) const noexcept
{
	return CreateWindowEx(
			0,
			TEXT("EDIT"),
			lpsz_lnk,
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			(INT)rc.left, (INT)rc.top,
			(INT)rc.right, (INT)rc.bottom,
			hwnd,
			(HMENU)WndCtrls::ID_EDIT_BOX,
			this->m_hinstance,
			NULL
		);
}

VOID WebBrowserWindow::UpdateLink() const
{
	if (sm_lpwb_wnd == nullptr) return;
	static BSTR s_bstr = nullptr;
	BSTR bstr;
	sm_lpwb_wnd->GetURL(&bstr);	
	if (s_bstr != bstr)
	{
		SetWindowText(this->m_hwnd_address_bar, bstr);
		s_bstr = bstr;
	}
	SysFreeString(bstr);
}

ErrorTypes WebBrowserWindow::GetErrorsState() const noexcept
{
	if (this->m_error_state != ErrorTypes::NULL_STATE)
		return this->m_error_state;
	return ErrorTypes::IS_OK;
}

ErrorTypes WebBrowserWindow::GetMonitorsState() const noexcept
{
	if (this->m_handle_monitors.Verify() != ErrorTypes::IS_OK)
		return ErrorTypes::TOTAL_ERROR;
	return ErrorTypes::IS_OK;
}

WebBrowserWindow* WebBrowserWindow::CreateSafe(HINSTANCE h_instance, LPCTSTR lpsz_link)
{
	// Verify link on validity
	Verifying::VerifyingLink check_link(lpsz_link);
 	if (check_link.Verify() == ErrorTypes::INCORRECT_LINK)
 		return nullptr;
 	// Verify session on remote or virtual access
	Handle::HandlingSession	 handle_session;
 	if (handle_session.Verify() != ErrorTypes::IS_OK)
 		return nullptr;
	// If is ok, return object
	return new WebBrowserWindow(h_instance, lpsz_link);
}

ErrorTypes WebBrowserWindow::StartAuthentication(HWND hwnd, const RECT& rc_client) noexcept
{
	if (this->m_auth_window.CreateAuthWindow(hwnd) == nullptr)
		return ErrorTypes::AUTH_ERROR;
	tlf_i << AT << "Start Authentication";
	return ErrorTypes::IS_OK;
}

ErrorTypes WebBrowserWindow::StartTestPassing(HWND hwnd, const RECT& rc_client) noexcept
{
	// Create window of our web browser
	// in client side case
	if ((sm_lpwb_wnd = new BrowserGUI::OleClientSideBrowserGUI(hwnd)) == nullptr)
		return ErrorTypes::TEST_PASS_ERROR;

	RECT rc = { 0, 45, rc_client.right, rc_client.bottom };
	sm_lpwb_wnd->SetRect(rc);
	return ErrorTypes::IS_OK;
}

LRESULT CALLBACK WebBrowserWindow::WndProc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param)
{	
	RECT		rc_logo = { 10, 13, 200, 20 }, rc_edit, rc_go;
	LONG		style;
	DWORD		ctrl_id;
	RECT		rc_client;	
	INT			status = 0;

	switch (u_msg)
	{
	/// Create window
	case WM_CREATE:
		if (this->InitHook() == ErrorTypes::SET_HOOK_ERROR)
		{
			this->m_error_state = ErrorTypes::SET_HOOK_ERROR;
			tlf_f << AT << ERROR_INIT_HOOK;
			FAILURE_BOX(ERROR_INIT_HOOK);			
		}
		this->m_lpkey_seq = std::make_unique<KS::KeySequence>();

		tlf_i << AT << "Hook init successfully";

		// Set window params
		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, NULL);
		style = GetWindowLong(hwnd, GWL_STYLE);
		style &= ~WS_MAXIMIZEBOX;
		style &= ~WS_MINIMIZEBOX;
		style &= ~WS_THICKFRAME;
		SetWindowLongPtr(hwnd, GWL_STYLE, style);
		ShowWindow(hwnd, SHOW_FULLSCREEN);

		tlf_i << AT << "Set window params successfully";

	/// STAGE1: Authentication of user
		if (this->StartAuthentication(hwnd, rc_client) == ErrorTypes::AUTH_ERROR)
		{
			this->m_error_state = ErrorTypes::AUTH_ERROR;
			tlf_f << AT << ERROR_AUTH_STAGE;
			FAILURE_BOX(ERROR_AUTH_STAGE);			
		}
		tlf_i << AT << "Authentication successfully";

	/// Create controls
		GetClientRect(hwnd, &rc_client);
		// Create label
		HWND label_handle;
		label_handle = this->CreateLabel(SB_LOGO_TEXT, rc_logo, hwnd);
		this->SetFont(label_handle, FONT_VERDANA, 13, FW_MEDIUM);
		// Button GO
		rc_go   = { rc_client.right - 90, 12, 75, 25 };
		this->CreateButton(BUTTON_GO, rc_go, hwnd);
		// Edit Box
		rc_edit = { 150, 12, rc_client.right - 250, 25 };
		this->m_hwnd_address_bar = this->CreateEditBox(TEXT(""), rc_edit, hwnd);
		this->SetFont(this->m_hwnd_address_bar, FONT_VERDANA, 11, FW_LIGHT);
		tlf_i << AT << "Create controls successfully";
	/// STAGE2: Test Passing
		if (this->StartTestPassing(hwnd, rc_client) == ErrorTypes::TEST_PASS_ERROR) 
		{
			this->m_error_state = ErrorTypes::TEST_PASS_ERROR;
			tlf_f << AT << ERROR_START_PASS;
			FAILURE_BOX(ERROR_START_PASS);			
		}

		return 0;
	case WM_ON_AUTHWND_CLOSED:
		if (this->m_lpkey_seq->SetMouseHook() == ErrorTypes::SET_HOOK_ERROR)
		{
			this->m_error_state = ErrorTypes::SET_HOOK_ERROR;
			tlf_f << AT << ERROR_INIT_MOUSE_HOOK;
			FAILURE_BOX(ERROR_INIT_MOUSE_HOOK);
		}
		if ((this->m_lpface_detector = new FD::FaceDetector(0)) == nullptr)
		{
			this->m_error_state = ErrorTypes::NULLPTR_STATE;
			tlf_f << AT << ERROR_INIT_FACE_DETECTOR;
			FAILURE_BOX(ERROR_INIT_FACE_DETECTOR);
		}
		// Right init of thread
		try
		{
			this->m_async_cam_thread = std::async(std::launch::async, &FD::FaceDetector::SaveWithInterval, 
					this->m_lpface_detector, 10);
		}
		catch (std::exception& e)
		{
			this->m_error_state = ErrorTypes::CATCH_EXCEPTION;
			tlf_f << AT << EXCEPTION << e.what();
			FAILURE_BOX(_EXCEPTION);
		}

		// Show test page
		sm_lpwb_wnd->Navigate(m_lpsz_link);

		return 0;
	case WM_COMMAND:
		if (LOWORD(w_param) == (INT)WndCtrls::ID_GO_BTN)
		{
			TCHAR* sz_url = new TCHAR[1024];
			GetWindowText(this->m_hwnd_address_bar, sz_url, 1024);
			sm_lpwb_wnd ->Navigate(sz_url);
		}
		return 0;
	case WM_CTLCOLORSTATIC:
		ctrl_id = GetDlgCtrlID((HWND)l_param);
		if (ctrl_id == (INT)WndCtrls::ID_LOGO)
		{
			SetBkColor((HDC)w_param, LIGHT_BLUE);
			SetTextColor((HDC)w_param, RGB(100, 100, 215));
			return (INT_PTR)CreateSolidBrush(LIGHT_BLUE);
		}
		
		return 0;
	// Prevent move wnd
	case WM_SYSCOMMAND:
		status = (INT)w_param & 0xfff0;
		if (status == SC_MOVE)
			break;
		else if (status == SC_CLOSE)
			return DefWindowProc(hwnd, u_msg, w_param, l_param);
		return 0;
	case WM_CLOSE:
		EnableWindow(hwnd, FALSE);
		if (WARNING_BOX(WANT_CLOSE_WB) == IDNO)
		{
			EnableWindow(hwnd, TRUE);
			SetFocus(hwnd);
			return 0;
		}

		this->m_lpface_detector->StopFaceDetector();
		delete this->m_lpface_detector;

		{
			Subject sb_subj;
			RestClient sb_client(&sb_subj);
			sb_client.SetConfigFile("RestClient//config.txt");
			sb_client.request("Log//info_file.log");
		}

		return DestroyWindow(hwnd);
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, u_msg, w_param, l_param);
	}
}

LRESULT FAR PASCAL WebBrowserWindow::GetMsgHookProc(int n_code, WPARAM w_param, LPARAM l_param)
{
	if (n_code >= 0 && PM_REMOVE == w_param)
	{
		MSG* p_msg = (MSG*)l_param;
		// Don't translate non-input events.
		if (p_msg->message >= WM_KEYFIRST && p_msg->message <= WM_KEYLAST)
		{
			if (p_msg != nullptr)
				sm_lpwb_wnd->DoPageAction(p_msg);
		}
	}
	return CallNextHookEx(NULL, n_code, w_param, l_param);
}


