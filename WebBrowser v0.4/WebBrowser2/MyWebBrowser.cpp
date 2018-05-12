#include "stdafx.h"
#include "MyWebBrowser.h"
#include "HelpTools.h"

using namespace ISXMyWebBrowser;

MyWebBrowser::MyWebBrowser(LPCTSTR lpsz_caption, LPWNDCLASSEX lpwn_class_ex, LPCTSTR lpsz_link)
		: MyWindow(lpsz_caption, lpwn_class_ex)
		, m_chk_link(lpsz_link)
{ }

MyWebBrowser::MyWebBrowser(HINSTANCE h_instance, LPCTSTR lpsz_link)
		: MyWindow(TEXT("WebBrowser"), h_instance, TEXT("WebBrowserWindow"))
		, m_chk_link(lpsz_link)
{
	if (this->m_chk_link.CheckProtocol() == FALSE) exit(EXIT_SUCCESS);
	if (this->m_chk_link.CheckDomain() == FALSE)   exit(EXIT_SUCCESS);
	this->m_lpsz_link = lpsz_link;
}

MyWebBrowser::~MyWebBrowser()
{
	delete this->m_lpwb_wnd;
}

VOID MyWebBrowser::CreateButton(LPCTSTR lpsz_name, HWND hwnd, HINSTANCE h_inst) const
{
	CreateWindowEx(
			0, 
			TEXT("BUTTON"),
			lpsz_name,
			WS_CHILD | WS_VISIBLE,
			10,  8, 
			80, 30,
			hwnd, 
			(HMENU)BTN_REFRESH,
			h_inst, 
			NULL
		);
}

HWND MyWebBrowser::CreateEditBox(LPCTSTR lpsz_lnk, HWND hwnd, HINSTANCE h_inst) const
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
			h_inst, 
			NULL
		);
}

VOID MyWebBrowser::UpdateWindow() const
{
	BSTR bstr;

	this->m_lpwb_wnd->GetURL(&bstr);
	SetWindowText(this->m_hwnd_address_bar, bstr);
	SysFreeString(bstr);
}

LRESULT CALLBACK MyWebBrowser::WndProc(HWND hwnd, UINT u_msg, WPARAM w_param,
		LPARAM l_param)
{
	INT		status;
	RECT	rc_client;
	LONG	style;

	switch (u_msg)
	{
	// Create window
	case WM_CREATE:
		// Create controls and disable editbox
		this->CreateButton(TEXT("Refresh"), hwnd, this->m_hinstance);
		this->m_hwnd_address_bar = this->CreateEditBox(TEXT(""), hwnd, this->m_hinstance);
		EnableWindow(this->m_hwnd_address_bar, FALSE);

		// Set window params
		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, NULL);
		style  =  GetWindowLong(hwnd, GWL_STYLE);
		style &= ~WS_MAXIMIZEBOX;
		style &= ~WS_MINIMIZEBOX;
		style &= ~WS_THICKFRAME;
		SetWindowLong(hwnd, GWL_STYLE, style);
		ShowWindow(hwnd, SHOW_FULLSCREEN);

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
		return 0;
	// Add functional to btn refresh
	case WM_COMMAND:
		switch (LOWORD(w_param))
		{
		case BTN_REFRESH:
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
		if (WARNING_BOX("You really want to close browser?") == IDNO)
			return 0;
		else
			return DestroyWindow(hwnd);
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, u_msg, w_param, l_param);
	}
}
