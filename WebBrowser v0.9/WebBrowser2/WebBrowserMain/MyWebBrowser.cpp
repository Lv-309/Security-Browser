#include "stdafx.h"
#include "HelpTools.h"
#include "VerifyLink.h"
#include "MyWebBrowser.h"
#include "HandleSession.h"

using ISXVerifyLink::VerifyLink;
using ISXHandleSession::HandleSession;
using namespace ISXMyWebBrowser;

MyWebBrowser::MyWebBrowser(LPCTSTR lpsz_caption, LPWNDCLASSEX lpwn_class_ex, LPCTSTR lpsz_link)
		: MyWindow(lpsz_caption, lpwn_class_ex)
		, m_handle_monitors()
		, m_lpsz_link(lpsz_link)
{ }

MyWebBrowser::MyWebBrowser(HINSTANCE h_instance, LPCTSTR lpsz_link)
		: MyWindow(TEXT("WebBrowser"), h_instance, TEXT("WebBrowserWindow"))
		, m_handle_monitors()
		, m_lpsz_link(lpsz_link)
{ }

MyWebBrowser::~MyWebBrowser()
{ }

HWND MyWebBrowser::CreateButton(LPCTSTR lpsz_name, HWND hwnd) const noexcept
{
	RECT rc = { 10, 8, 80, 30 };
	return this->CreateButton(
			lpsz_name,
			hwnd,
			rc,
			(HMENU)WndControls::ID_REFRESH_BTN
		);
}

HWND MyWebBrowser::CreateButton(LPCTSTR lpsz_name, HWND hwnd, RECT rc, HMENU menu) const noexcept
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

HWND MyWebBrowser::CreateEditBox(LPCTSTR lpsz_lnk, HWND hwnd) const noexcept
{
	return CreateWindowEx(
			0, 
			TEXT("EDIT"),
			lpsz_lnk,
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			100, 12,
			450, 20,
			hwnd, 
			NULL, 
			this->m_hinstance,
			NULL
		);
}

VOID MyWebBrowser::Update() const
{
	BSTR bstr;
	this->m_lpwb_wnd->GetURL(&bstr);
	SetWindowText(this->m_hwnd_address_bar, bstr);
	SysFreeString(bstr);
}

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
	if (handle_session.Verify() != ErrorTypes::IS_OK)
		return nullptr;
	// If is ok, return object
	return new MyWebBrowser(h_instance, lpsz_link);
}

ErrorTypes MyWebBrowser::Authentication(RECT rc, HWND hwnd) const noexcept
{
	// Set position of controls 
	RECT rc_webcam_wnd = { (rc.right / 2) - 320, (rc.bottom / 2) - 240, 640L, 480L };
	RECT rc_photo_btn  = { rc.right - (LONG)(rc.right * 0.32), rc.bottom - (LONG)(rc.bottom * 0.2), 113L, 30L };
	RECT rc_reset_btn  = { rc.right - (LONG)(rc.right * 0.25), rc.bottom - (LONG)(rc.bottom * 0.2),  80L, 30L };
	RECT rc_submit_btn = { rc.right - (LONG)(rc.right * 0.2),  rc.bottom - (LONG)(rc.bottom * 0.2),  80L, 30L };
	// Register child window
	this->Register(this->AuthChildProc, TEXT("WebCamChildClass"), this->m_hinstance);
	// Create child window
	HWND child = CreateWindowEx(0, TEXT("WebCamChildClass"), (LPCTSTR)NULL, WS_CHILD | WS_BORDER | WS_VISIBLE,
		(INT)rc_webcam_wnd.left, (INT)rc_webcam_wnd.top, (INT)rc_webcam_wnd.right, (INT)rc_webcam_wnd.bottom,
		hwnd, (HMENU)(INT)WndControls::ID_WEBCAM_CHILD, this->m_hinstance, NULL);
	// Show
	ShowWindow(child, SW_NORMAL);
	// Create buttons
	this->CreateButton(TEXT("Make a photo"), hwnd, rc_photo_btn, (HMENU)WndControls::ID_PHOTO_BTN);
	this->CreateButton(TEXT("Reset"), hwnd, rc_reset_btn, (HMENU)WndControls::ID_RESET_BTN);
	this->CreateButton(TEXT("Submit"), hwnd, rc_submit_btn, (HMENU)WndControls::ID_SUBMIT_BTN);

	return ErrorTypes::IS_OK;
}

LRESULT CALLBACK MyWebBrowser::WndProc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param)
{
	RECT	rc_client;
	LONG	style;

	switch (u_msg)
	{
	// Create window
	case WM_CREATE:

		// Create controls and disable editbox
		this->CreateButton(TEXT("Refresh"), hwnd);
		this->m_hwnd_address_bar = this->CreateEditBox(TEXT(""), hwnd);
		EnableWindow(this->m_hwnd_address_bar, FALSE);

		// Set window params
		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, NULL);
		style  =  GetWindowLong(hwnd, GWL_STYLE);
		style &= ~WS_MAXIMIZEBOX;
		style &= ~WS_MINIMIZEBOX;
		style &= ~WS_THICKFRAME;
		SetWindowLong(hwnd, GWL_STYLE, style);
		ShowWindow(hwnd, SHOW_FULLSCREEN);


		GetClientRect(hwnd, &rc_client);

		this->Authentication(rc_client, hwnd);
		

		#if 0
		// Create window of our web browser
		// in client side case
		GetClientRect(hwnd, &rc_client);
		this->m_lpwb_wnd = new WebBrowserWindow(hwnd);
		RECT rc;
		rc.left   = 0;
		rc.top    = 45;
		rc.right  = rc_client.right;
		rc.bottom = rc_client.bottom;
		this->m_lpwb_wnd->SetRect(rc);
		this->m_lpwb_wnd->Navigate(m_lpsz_link);
		#endif

		return 0;
	// Add functional to btn refresh
	case WM_COMMAND:
		switch (LOWORD(w_param))
		{
		case (INT)WndControls::ID_REFRESH_BTN:
			m_lpwb_wnd->Refresh();
			return 0;
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
		else
			return DestroyWindow(hwnd);
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, u_msg, w_param, l_param);
	}
}

LRESULT CALLBACK MyWebBrowser::AuthChildProc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param)
{
	if (u_msg == WM_DESTROY)
	{
		return 0;
	}
	return DefWindowProc(hwnd, u_msg, w_param, l_param);
}
