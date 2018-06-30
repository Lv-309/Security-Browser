#include <thread>

#include "stdafx.h"
#include "HelpTools.h"
#include "VerifyLink.h"
#include "MyWebBrowser.h"
#include "HandleSession.h"
#include "..\WebCamera\Frame.h"
#include "..\WebCamera\FaceDetector.h"
#include "..\TraceUtility\LogInfo.h"
#include "..\TraceUtility\LogFatal.h"

using ISXVerifyLink::VerifyLink;
using ISXHandleSession::HandleSession;
using namespace ISXMyWebBrowser;

HHOOK						MyWebBrowser::sm_msg_hook = nullptr;
CComPtr<WebBrowserWindow>	MyWebBrowser::sm_lpwb_wnd = nullptr;

MyWebBrowser::MyWebBrowser(HINSTANCE h_instance, LPCTSTR lpsz_link)
	: MyWindow(TEXT("WebBrowser"), h_instance, TEXT("WebBrowserWindow"))
	, m_handle_monitors()
	, m_lpsz_link(lpsz_link)
	, m_auth(this->m_hinstance)
{ }

MyWebBrowser::~MyWebBrowser()
{ }

BOOL MyWebBrowser::InitHook()
{
	sm_msg_hook = SetWindowsHookEx(WH_GETMESSAGE, GetMsgHookProc, NULL, GetCurrentThreadId());
	return TRUE;
}
VOID MyWebBrowser::SetFont(const HWND handle, LPCTSTR font_name, SHORT font_size, LONG font_weight) const noexcept
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

HWND MyWebBrowser::CreateLabel(LPCTSTR lpsz_text, const RECT& rc, HWND hwnd) const noexcept
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

HWND MyWebBrowser::CreateButton(LPCTSTR lpsz_name, const RECT& rc, HWND hwnd) const noexcept
{
	return this->CreateButton(
		lpsz_name,
		hwnd,
		rc,
		(HMENU)WndCtrls::ID_GO_BTN
	);
}

HWND MyWebBrowser::CreateButton(LPCTSTR lpsz_name, HWND hwnd, const RECT& rc, HMENU menu) const noexcept
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

HWND MyWebBrowser::CreateEditBox(LPCTSTR lpsz_lnk, const RECT& rc, HWND hwnd) const noexcept
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

VOID MyWebBrowser::UpdateLink() const
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

ErrorTypes MyWebBrowser::Verify() const noexcept
{
	if (this->m_handle_monitors.Verify() != ErrorTypes::IS_OK)
		return ErrorTypes::TOTAL_ERROR;
	return ErrorTypes::IS_OK;
}

MyWebBrowser* MyWebBrowser::CreateSafe(HINSTANCE h_instance, LPCTSTR lpsz_link)
{
	// Verify link on validity
 	VerifyLink		check_link(lpsz_link);
 	if (check_link.Verify() != ErrorTypes::IS_OK)
 		return nullptr;
 	// Verify session on remote or virtual access
 	HandleSession	handle_session;
 	if (handle_session.Verify() != ErrorTypes::IS_OK)
 		return nullptr;
	// If is ok, return object
	return new MyWebBrowser(h_instance, lpsz_link);
}

ErrorTypes MyWebBrowser::Authentication(HWND hwnd, const RECT& rc_client) noexcept
{
	if (this->m_auth.CreateAuthWindow(hwnd) == nullptr)
		return ErrorTypes::TOTAL_ERROR;
	return ErrorTypes::IS_OK;
}

ErrorTypes MyWebBrowser::TestPassing(HWND hwnd, const RECT& rc_client) noexcept
{
	// Create window of our web browser
	// in client side case
	sm_lpwb_wnd = new WebBrowserWindow(hwnd);
	RECT rc = { 0, 45, rc_client.right, rc_client.bottom };
	sm_lpwb_wnd->SetRect(rc);
	return ErrorTypes::IS_OK;
}

LRESULT CALLBACK MyWebBrowser::WndProc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param)
{	
	RECT		rc_logo = { 10, 13, 200, 20 }, rc_edit, rc_go;
	LONG		style;
	DWORD		ctrl_id;
	RECT		rc_client;	

	switch (u_msg)
	{
	// Create window
	case WM_CREATE:
		this->InitHook();
		this->m_lpkey_seq = std::make_unique<KeySequence>();
		// Set window params
		SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, NULL);
		style = GetWindowLong(hwnd, GWL_STYLE);
		style &= ~WS_MAXIMIZEBOX;
		style &= ~WS_MINIMIZEBOX;
		style &= ~WS_THICKFRAME;
		SetWindowLongPtr(hwnd, GWL_STYLE, style);
		ShowWindow(hwnd, SHOW_FULLSCREEN);

	// STAGE1: Authentication of user
		if (this->Authentication(hwnd, rc_client) != ErrorTypes::IS_OK)
			PostMessage(hwnd, WM_CLOSE, NULL, NULL); // TODO: do right exit

	// Create controls
		GetClientRect(hwnd, &rc_client);
		// Create label
		HWND label_handle;
		label_handle = this->CreateLabel(TEXT("Secure Browser"), rc_logo, hwnd);
		this->SetFont(label_handle, TEXT("Verdana"), 13, FW_MEDIUM);
		// Button GO
		rc_go   = { rc_client.right - 90, 12, 75, 25 };
		this->CreateButton(TEXT("Go"), rc_go, hwnd);
		// Edit Box
		rc_edit = { 150, 12, rc_client.right - 250, 25 };
		this->m_hwnd_address_bar = this->CreateEditBox(TEXT(""), rc_edit, hwnd);
		this->SetFont(this->m_hwnd_address_bar, TEXT("Verdana"), 11, FW_LIGHT);

		// STAGE2: Test Passing
		this->TestPassing(hwnd, rc_client);

		return 0;
	case WM_ON_AUTHWND_CLOSED:
		// Start thread for face detector
		// t = std::thread(&ISXFaceDetector::FaceDetector::SaveConstantly, &fd);

		this->m_lpkey_seq->SetMouseHook();

		this->m_lpface_detector = new FaceDetector(0);
		this->m_cum_thread = std::thread(&ISXFaceDetector::FaceDetector::SaveWithInterval, this->m_lpface_detector, 1);

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
	//case WM_SYSCOMMAND:
	//	status = (INT)w_param & 0xfff0;
	//	if (status == SC_MOVE)
	//		return 0;
	//	else if (status == SC_CLOSE)
	//		return DefWindowProc(hwnd, u_msg, w_param, l_param);
	case WM_CLOSE:
		EnableWindow(hwnd, FALSE);
		if (WARNING_BOX("You really want to close browser?") == IDNO)
		{
			EnableWindow(hwnd, TRUE);
			SetFocus(hwnd);
			return 0;
		}

		this->m_lpface_detector->StopFaceDetector();
		this->m_cum_thread.join();
 		delete this->m_lpface_detector;

		return DestroyWindow(hwnd);
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, u_msg, w_param, l_param);
	}
}

LRESULT FAR PASCAL MyWebBrowser::GetMsgHookProc(int n_code, WPARAM w_param, LPARAM l_param)
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
	return CallNextHookEx(sm_msg_hook, n_code, w_param, l_param);
}


