#include "stdafx.h"
#include "HelpTools.h"
#include "VerifyLink.h"
#include "MyWebBrowser.h"
#include "HandleSession.h"
#include "..\TraceUtility\LogInfo.h"
#include "..\TraceUtility\LogFatal.h"

using ISXVerifyLink::VerifyLink;
using ISXHandleSession::HandleSession;
using namespace ISXKeySequence;
using namespace ISXMyWebBrowser;

HHOOK						MyWebBrowser::sm_msg_hook = nullptr;
CComPtr<WebBrowserWindow>	MyWebBrowser::sm_lpwb_wnd = nullptr;

MyWebBrowser::MyWebBrowser(HINSTANCE h_instance, LPCTSTR lpsz_link)
	: MyWindow(TEXT("WebBrowser"), h_instance, TEXT("WebBrowserWindow"))
	, m_handle_monitors()
	, m_lpsz_link(lpsz_link)
	, m_cc_wnd(this->m_hinstance)
{ }

MyWebBrowser::~MyWebBrowser()
{ }

ErrorTypes MyWebBrowser::InitHook() const noexcept
{
	this->sm_msg_hook = SetWindowsHookEx(WH_GETMESSAGE, GetMsgHookProc, this->m_hinstance, GetCurrentThreadId());
	return ErrorTypes::IS_OK;
}

HWND MyWebBrowser::CreateLabel(LPCTSTR lpsz_text, const RECT& rc, HWND hwnd) const noexcept
{
	return CreateWindow(
		TEXT("STATIC"),
		lpsz_text,
		WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		rc.left, rc.top, rc.right, rc.bottom,
		hwnd,
		(HMENU)WndControls::ID_LOGO,
		this->m_hinstance,
		NULL
	);
}

HWND MyWebBrowser::CreateButton(LPCTSTR lpsz_name, const RECT& rc, HWND hwnd) const noexcept
{
	return this->CreateButton(
		lpsz_name,
		rc,
		(HMENU)WndControls::ID_GO_BTN,
		hwnd
	);
}

HWND MyWebBrowser::CreateButton(LPCTSTR lpsz_name, const RECT& rc, HMENU menu, HWND hwnd) const noexcept
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
			(HMENU)WndControls::ID_EDIT_BOX,
			this->m_hinstance,
			NULL
		);
}

VOID MyWebBrowser::UpdateLink() const
{
	static BSTR s_bstr = nullptr;
	BSTR bstr;
	this->sm_lpwb_wnd->GetURL(&bstr);
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
	//	if (handle_session.Verify() != ErrorTypes::IS_OK)
	//		return nullptr;
	// If is ok, return object
	return new MyWebBrowser(h_instance, lpsz_link);
}

ErrorTypes MyWebBrowser::Authentication(HWND hwnd, const RECT& rc_client) noexcept
{
	/// Model
	// Set position of controls 
	//RECT rc_webcam_wnd = { (rc.right / 2) - 320, (rc.bottom / 2) - 240, 640L, 480L };
	//IDCreator idc(rc_webcam_wnd, hwnd);
	//std::vector<std::wstring> vwstr = idc.GetNamesOfAvailableCameras();
	//if (idc.GetCamCount() >= 2)
	//{
	//	ChooseCamera cc;
	//	for (size_t i = 0; i < idc.GetCamCount(); ++i)
	//		cc.AddItem(vwstr.at(i).c_str());
	//	cc.Create();
	//	idc.SetCameraIndex(cc.GetSelectedItem());
	//}

	std::vector<std::wstring> vwstr = { L"Camera1", L"Camera2" , L"Camera3" };
	this->m_cc_wnd.Create();
	this->m_cc_wnd.AddItems(vwstr);
	int i = this->m_cc_wnd.GetSelectedIndex();

	return ErrorTypes::IS_OK;
}

ErrorTypes MyWebBrowser::TestPassing(HWND hwnd, const RECT& rc_client) noexcept
{
	RECT rc = { 0, 45, rc_client.right, rc_client.bottom };
	// Create window of our web browser in client side case
	// 	this->m_lpwb_wnd = new WebBrowserWindow(hwnd);
	// 	this->m_lpwb_wnd->SetRect(rc);
	// 	this->m_lpwb_wnd->Navigate(m_lpsz_link);

	this->sm_lpwb_wnd = new WebBrowserWindow(hwnd);
	this->sm_lpwb_wnd->SetRect(rc);
	this->sm_lpwb_wnd->Navigate(m_lpsz_link);

	tlf_i << AT << "User start test passing";
	return ErrorTypes::IS_OK;
}

LRESULT CALLBACK MyWebBrowser::WndProc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param)
{
	RECT		rc_client;
	RECT		rc_logo = { 10, 13, 200, 20 }, rc_edit, rc_go;
	LONG		style;
	DWORD		ctrl_id;


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

		GetClientRect(hwnd, &rc_client);

		// Create controls
		HWND label_handle;
		label_handle = this->CreateLabel(TEXT("Secure Browser"), rc_logo, hwnd);
		this->SetFont(label_handle, TEXT("Verdana"), 13, FW_MEDIUM);
		rc_go = { rc_client.right - 90, 12, 75, 25 };
		this->CreateButton(TEXT("Go"), rc_go, hwnd);
		rc_edit = { 150, 12, rc_client.right - 250, 25 };
		this->m_hwnd_address_bar = this->CreateEditBox(TEXT(""), rc_edit, hwnd);
		this->SetFont(this->m_hwnd_address_bar, TEXT("Verdana"), 11, FW_LIGHT);

		// EnableWindow(this->m_hwnd_address_bar, FALSE);

		//if (this->Authentication(hwnd, rc_client) != ErrorTypes::IS_OK)
		//	PostMessage(hwnd, WM_CLOSE, NULL, NULL); // TODO: do right exit

		this->TestPassing(hwnd, rc_client);
		return 0;
	case WM_CTLCOLORSTATIC:
		ctrl_id = GetDlgCtrlID((HWND)l_param);
		if (ctrl_id == (INT)WndControls::ID_LOGO)
		{
			SetBkColor((HDC)w_param, LIGHT_BLUE);
			SetTextColor((HDC)w_param, RGB(100, 100, 215));
			return (INT_PTR)CreateSolidBrush(LIGHT_BLUE);
		}
		return 0;
	case WM_COMMAND:
		if (LOWORD(w_param) == (INT)WndControls::ID_GO_BTN)
		{
			TCHAR* sz_url = new TCHAR[1024];
			GetWindowText(this->m_hwnd_address_bar, sz_url, 1024);
			sm_lpwb_wnd->Navigate(sz_url);
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
		return DestroyWindow(hwnd);
	case WM_DESTROY:
		UnhookWindowsHookEx(sm_msg_hook);
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, u_msg, w_param, l_param);
	}
}

LRESULT CALLBACK MyWebBrowser::GetMsgHookProc(int n_code, WPARAM w_param, LPARAM l_param)
{

	if (n_code >= 0 && PM_REMOVE == w_param)
	{
		MSG* p_msg = (MSG*)l_param;
		// Don't translate non-input events.

		MSLLHOOKSTRUCT &msll = *(reinterpret_cast<MSLLHOOKSTRUCT*>(l_param));

		if (w_param == WM_RBUTTONDOWN)
		{
			return -1; // Make this click be ignored
		}

// 		BOOL ñontrol_keydown = FALSE;
// 		ñontrol_keydown = GetAsyncKeyState(VK_CONTROL);
// 
// 		if (ñontrol_keydown && p_msg->wParam == 0x43)
// 		{
// 			/*SendMessage(hwndEdit, EM_SETSEL, 0, -1);*/
// 			return 1;
// 		}

		if (p_msg->message >= WM_KEYFIRST && p_msg->message <= WM_KEYLAST)
		{
			if (p_msg != nullptr)
				sm_lpwb_wnd->DoPageAction(p_msg);
		}

	}

	return CallNextHookEx(NULL, n_code, w_param, l_param);
}