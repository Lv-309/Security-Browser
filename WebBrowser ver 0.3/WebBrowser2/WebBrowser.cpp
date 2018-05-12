#include "stdafx.h"
#include "WebBrowser.h"
#include "HelpTools.h"

using namespace ISXMyWebBrowser;

MyWebBrowser::MyWebBrowser(LPCTSTR lpsz_caption, LPWNDCLASSEX lpwn_class_ex)
		: MyWindow(lpsz_caption, lpwn_class_ex)
{ }

MyWebBrowser::MyWebBrowser(LPCTSTR lpsz_caption, HINSTANCE h_instance, LPCTSTR lpsz_class_name, LPWSTR lpwstr_startPage)
		: MyWindow(lpsz_caption, h_instance, lpsz_class_name), m_lpwstr_startPage(lpwstr_startPage)
{ }

MyWebBrowser::~MyWebBrowser()
{
	delete m_lpwb_wnd;
}

VOID MyWebBrowser::CreateButton(LPCWSTR btn_name, HWND hwnd, HINSTANCE hinst) const
{
	CreateWindowEx(
			0, 
			TEXT("BUTTON"),
			btn_name,
			WS_CHILD | WS_VISIBLE,
			10,  8, 
			80, 30,
			hwnd, 
			(HMENU)BTN_REFRESH,
			hinst, 
			NULL
		);
}

HWND MyWebBrowser::CreateEditBox(LPCWSTR link, HWND hwnd, HINSTANCE hinst) const
{
	return CreateWindowEx(
			0, 
			TEXT("EDIT"),
			TEXT(""),
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			100, 12,
			300, 20,
			hwnd, 
			NULL, 
			hinst, 
			NULL
		);
}

LRESULT CALLBACK MyWebBrowser::WndProc(HWND hwnd, UINT u_msg, WPARAM w_param,
		LPARAM l_param)
{
	INT		status;
	RECT	rc_client;
	LONG	style;
	HWND	hwnd_address_bar;
	BSTR	bstr;

	switch (u_msg)
	{
	// Create window
	case WM_CREATE:
	{
		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, NULL);		//HWND_TOPMOST
		style = GetWindowLong(hwnd, GWL_STYLE);
		style &= ~WS_MAXIMIZEBOX;
		style &= ~WS_MINIMIZEBOX;
		SetWindowLong(hwnd, GWL_STYLE, style);
		ShowWindow(hwnd, SHOW_FULLSCREEN);
		// Create window of our web browser
		// in client side case
		GetClientRect(hwnd, &rc_client);
		m_lpwb_wnd = new WebBrowserWindow(hwnd);

		m_lpwb_wnd->Navigate(m_lpwstr_startPage);

		RECT rc;
		rc.left = 0;
		rc.top = 45;
		rc.right = rc_client.right;
		rc.bottom = rc_client.bottom;
		m_lpwb_wnd->SetRect(rc);
		// Getting current URL
		this->m_lpwb_wnd->GetURL(&bstr);
		// Create controls
		this->CreateButton(TEXT("Refresh"), hwnd, this->m_hinstance);
		hwnd_address_bar = this->CreateEditBox(bstr, hwnd, this->m_hinstance);
		return 0;
	}
	// Add function to btn refresh
	case WM_COMMAND:
		switch (LOWORD(w_param))
		{
		case BTN_REFRESH:
			m_lpwb_wnd->Refresh();
			return 0;
		}
		return 0;
	// Prevent move wnd
	case WM_SYSCOMMAND:
		status = (INT)w_param & 0xfff0;
		if (status == SC_MOVE)
			return 0;
		else if (status == SC_CLOSE)
			return DefWindowProc(hwnd, u_msg, w_param, l_param);
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
