#include "stdafx.h"
#include "MyWebBrowser.h"
#include "HelpTools.h"

#define ID_AUTH_CHILD 100
#define ID_VERIFY_CHILD 101
#define ID_SUBMIT_BTN 102
#define ID_RESET_BTN 103
#define ID_PHOTO_BTN 104

using namespace ISXMyWebBrowser;

#include "ChildWindow.h"
using namespace ISXChildWindow;

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

VOID MyWebBrowser::CreateButton(LPCTSTR lpsz_name, HWND hwnd, HINSTANCE h_inst, INT x, INT y, INT w, INT h, HMENU menu) const
{
	CreateWindowEx(
			0,
			TEXT("BUTTON"),
			lpsz_name,
			WS_CHILD | WS_VISIBLE,
			x, y,
			w, h,
			hwnd,
			menu,
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

VOID MyWebBrowser::Update() const
{
	BSTR bstr;
	this->m_lpwb_wnd->GetURL(&bstr);
	SetWindowText(this->m_hwnd_address_bar, bstr);
	SysFreeString(bstr);
}

ErrorTypes MyWebBrowser::Verify() const
{
	if (this->m_handle_monitors.Verify() != ErrorTypes::ISOK)
		return ErrorTypes::TOTALERROR;
	return ErrorTypes::ISOK;
}

// Factory
MyWebBrowser* MyWebBrowser::CreateSafe(HINSTANCE h_instance, LPCTSTR lpsz_link)
{
	// Verify link on validity
	VerifyLink		check_link(lpsz_link);
	if (check_link.Verify() != ErrorTypes::ISOK)
		return nullptr;
	// Verify session on remote or virtual access
	HandleSession	handle_session;
	if (handle_session.Verify() != ErrorTypes::ISOK)
		return nullptr;
	// If is ok, return object
	return new MyWebBrowser(h_instance, lpsz_link);
}

VOID MyWebBrowser::Authentication(RECT rc_client, HWND hwnd)
{

	RECT rc_wc;
	rc_wc.left = (rc_client.right / 2) - 320;
	rc_wc.top = (rc_client.bottom / 2) - 240;
	rc_wc.right = 640;
	rc_wc.bottom = 480;

	
	std::unique_ptr<ChildWindow> child_wc;
	child_wc = std::make_unique<ChildWindow>(hwnd, this->m_hinstance, TEXT("Child2"), rc_wc);
	this->CreateButton(TEXT("Make a photo"), hwnd, this->m_hinstance, rc_client.right - (rc_client.right * 0.32),
		rc_client.bottom - (rc_client.bottom * 0.2), 115, 30, (HMENU)ID_PHOTO_BTN);
	this->CreateButton(TEXT("Reset"), hwnd, this->m_hinstance, rc_client.right - (rc_client.right * 0.25),
		rc_client.bottom - (rc_client.bottom * 0.2), 80, 30, (HMENU)ID_RESET_BTN);
	this->CreateButton(TEXT("Submit"), hwnd, this->m_hinstance, rc_client.right - (rc_client.right * 0.2),
		rc_client.bottom - (rc_client.bottom * 0.2), 80, 30, (HMENU)ID_SUBMIT_BTN);
}

LRESULT CALLBACK MyWebBrowser::WndProc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param)
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


		GetClientRect(hwnd, &rc_client);

		this->Authentication(rc_client, hwnd);

// 		RECT rc_bg;
// 		rc_bg.left = 0;
// 		rc_bg.top = 45;
// 		rc_bg.right = rc_client.right;
// 		rc_bg.bottom = rc_client.bottom;
// 
// 		
// 		ChildWindow* child_bg;
// 		child_bg = new ChildWindow(hwnd, this->m_hinstance, TEXT("Child1"), rc_bg);
// 
// 		RECT rc_bg2;
// 		rc_bg2.left = (rc_client.right / 2) - 320;
// 		rc_bg2.top = (rc_client.bottom / 2) - 240;
// 		rc_bg2.right = 640;
// 		rc_bg2.bottom = 480;
// 
// 		ChildWindow* child_bg2;
// 		child_bg2 = new ChildWindow(hwnd, this->m_hinstance, TEXT("Child2"), rc_bg2);

// 		WNDCLASSEX wcex_child;
// 		wcex_child.cbSize = sizeof(WNDCLASSEX);
// 		wcex_child.cbClsExtra = 0;
// 		wcex_child.cbWndExtra = 0;
// 		wcex_child.style = CS_HREDRAW | CS_VREDRAW;
// 		wcex_child.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
// 		wcex_child.hCursor = LoadCursor(NULL, IDC_ARROW);
// 		wcex_child.hIcon = LoadIcon(m_hinstance, MAKEINTRESOURCE(IDI_WEBBROWSER2));
// 		wcex_child.hIconSm = LoadIcon(m_hinstance, MAKEINTRESOURCE(IDI_SMALL));
// 		wcex_child.lpszMenuName = NULL;
// 		wcex_child.lpfnWndProc = this->AuthChildProc;
// 		wcex_child.lpszClassName = TEXT("AuthChildClass");
// 		wcex_child.hInstance = this->m_hinstance;
// 		RegisterClassEx(&wcex_child);
// 
// 		HWND auth_child;
// 		GetClientRect(hwnd, &rc_client);
// 		
// 		auth_child = CreateWindowEx(
// 			0,
// 			TEXT("AuthChildClass"),
// 			(LPCTSTR)NULL,
// 			WS_CHILD | WS_BORDER | WS_VISIBLE,
// 			0, 45,
// 			rc_client.right, rc_client.bottom,
// 			hwnd,
// 			(HMENU)(INT)ID_AUTH_CHILD,
// 			this->m_hinstance,
// 			NULL
// 		);
// 		ShowWindow(auth_child, SW_NORMAL);
		

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
