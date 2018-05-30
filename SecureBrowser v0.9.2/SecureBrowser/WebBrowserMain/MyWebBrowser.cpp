#include "stdafx.h"
#include "HelpTools.h"
#include "VerifyLink.h"
#include "MyWebBrowser.h"
#include "HandleSession.h"
#include "..\TraceUtility\LogInfo.h"
#include "..\TraceUtility\LogFatal.h"

using ISXVerifyLink::VerifyLink;
using ISXHandleSession::HandleSession;
using namespace ISXMyWebBrowser;

MyWebBrowser::MyWebBrowser(HINSTANCE h_instance, LPCTSTR lpsz_link)
		: MyWindow(TEXT("WebBrowser"), h_instance, TEXT("WebBrowserWindow"))
		, m_handle_monitors()
		, m_lpsz_link(lpsz_link)
		, m_cc_wnd(this->m_hinstance)
{ }

MyWebBrowser::~MyWebBrowser()
{ 
	if (this->m_hwnd_self != nullptr)
	{
		DestroyWindow(this->m_hwnd_self);
		this->m_hwnd_self = nullptr;
	}
}

HWND MyWebBrowser::CreateLabel(LPCTSTR lpsz_text, const RECT& rc, HWND hwnd) const noexcept
{
	return CreateWindow(TEXT("STATIC"), lpsz_text, WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		rc.left, rc.top, rc.right, rc.bottom, 
		hwnd, (HMENU)WndControls::ID_LOGO, this->m_hinstance, NULL);
}

HWND MyWebBrowser::CreateButton(LPCTSTR lpsz_name, HWND hwnd) const noexcept
{
	RECT rc = { 10, 8, 80, 30 };
	return this->CreateButton(
			lpsz_name,
			hwnd,
			rc,
			(HMENU)WndControls::ID_GO_BTN
		);
}

HWND MyWebBrowser::CreateButton(LPCTSTR lpsz_name, HWND hwnd, const RECT& rc, HMENU menu) const noexcept
{
	return CreateWindowEx(
			0,
			TEXT("BUTTON"),
			lpsz_name,
			WS_CHILD | WS_VISIBLE,
			(INT)rc.left,  (INT)rc.top,
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
			NULL, 
			this->m_hinstance,
			NULL
		);
}

// VOID MyWebBrowser::UpdateLink() const
// {
// 	BSTR bstr;
// 	this->m_lpwb_wnd->GetURL(&bstr);
// 	SetWindowText(this->m_hwnd_address_bar, bstr);
// 	SysFreeString(bstr);
// }

ErrorTypes MyWebBrowser::Verify() const noexcept
{
	if (this->m_handle_monitors.Verify() != ErrorTypes::IS_OK)
		return ErrorTypes::TOTAL_ERROR;
	return ErrorTypes::IS_OK;
}

// Factory
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
	// Create window of our web browser
	// in client side case
	this->m_lpwb_wnd = new WebBrowserWindow(hwnd);
	RECT rc = { 0, 45, rc_client.right, rc_client.bottom };;
	this->m_lpwb_wnd->SetRect(rc);
	this->m_lpwb_wnd->Navigate(m_lpsz_link);

	tlf_i << AT << "User start test passing";
	return ErrorTypes::IS_OK;
}

LRESULT CALLBACK MyWebBrowser::WndProc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param)
{
	RECT		rc_client, rc_edit, rc_logo = { 10, 15, 120, 20 };
	LONG		style;
	DWORD		label_id;

	this->m_hwnd_self = hwnd;

	switch (u_msg)
	{
	// Create window
	case WM_CREATE:
		// Set window params
		SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, NULL);
		style  =  GetWindowLong(hwnd, GWL_STYLE);
		style &= ~WS_MAXIMIZEBOX;
		style &= ~WS_MINIMIZEBOX;
		style &= ~WS_THICKFRAME;
		SetWindowLongPtr(hwnd, GWL_STYLE, style);
		ShowWindow(hwnd, SHOW_FULLSCREEN);

		GetClientRect(hwnd, &rc_client);
		// Create controls and disable editbox
		this->CreateLabel(TEXT("Secure Browser"), rc_logo, hwnd);
		//this->CreateButton(TEXT("Go"), hwnd);
		// Edit
		rc_edit = { 150, 12, rc_client.right - 250, 25 };
		this->m_hwnd_address_bar = this->CreateEditBox(TEXT(""), rc_edit, hwnd);
		EnableWindow(this->m_hwnd_address_bar, FALSE);

		if (this->Authentication(hwnd, rc_client) != ErrorTypes::IS_OK)
				PostMessage(hwnd, WM_CLOSE, NULL, NULL); // TODO: do right exit

		this->TestPassing(hwnd, rc_client);
		return 0;
	case WM_CTLCOLORSTATIC:
		label_id = GetDlgCtrlID((HWND)l_param);
		if (label_id == (INT)WndControls::ID_LOGO)
		{
			SetBkColor((HDC)w_param, LIGHT_BLUE);
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
		}
		else
			DestroyWindow(hwnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, u_msg, w_param, l_param);
	}
}

