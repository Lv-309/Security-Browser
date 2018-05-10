#include "stdafx.h"
#include "MyWindow.h"

using namespace ISXMyWindow;

MyWindow::MyWindow(LPCTSTR lpsz_caption, LPWNDCLASSEX lpwn_class_ex)
		: m_hinstance(lpwn_class_ex->hInstance)
{
	this->SetCaption(lpsz_caption);
	this->SetClassName(lpwn_class_ex->lpszClassName);
	this->Register(lpwn_class_ex);
}

MyWindow::MyWindow(LPCTSTR lpsz_caption, HINSTANCE h_hinstance, LPCTSTR lpwn_class)
		: m_hinstance(h_hinstance)
{
	this->SetCaption(lpsz_caption);
	this->SetClassName(lpwn_class);
	
	WNDCLASSEX wcex;
	wcex.cbSize			=  sizeof(WNDCLASSEX);
	wcex.cbClsExtra		=  0;
	wcex.cbWndExtra		=  0;
	wcex.style			=  CS_HREDRAW | CS_VREDRAW;
	wcex.hbrBackground	=  (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wcex.hCursor		=  LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon			=  LoadIcon(m_hinstance, MAKEINTRESOURCE(IDI_WEBBROWSER2));
	wcex.hIconSm		=  LoadIcon(m_hinstance, MAKEINTRESOURCE(IDI_SMALL));
	wcex.lpszMenuName	=  NULL;

	if (this->Register(&wcex) == FALSE)
		throw "Cannot register main window class";
}

ATOM MyWindow::Register(LPWNDCLASSEX wcex)
{
	wcex->lpfnWndProc	=  this->MsgRouter;
	wcex->lpszClassName =  this->m_lpclass_name;
	wcex->hInstance		=  this->m_hinstance;

	return RegisterClassEx(wcex);
}

MyWindow::~MyWindow()
{
	if (this->m_hwnd != NULL) 
	{
		DestroyWindow(this->m_hwnd);
		this->m_hwnd = NULL;
	}

	if (this->m_lpclass_name != NULL) 
	{
		free((LPVOID)this->m_lpclass_name);
		this->m_lpclass_name = NULL;
	}

	if (this->m_lpcaption != NULL) 
	{
		free((LPVOID)this->m_lpcaption);
		this->m_lpcaption = NULL;
	}
}

VOID MyWindow::SetCaption(LPCTSTR lpsz_caption)
{
	if (lpsz_caption == NULL && this->m_hwnd != NULL) 
		throw "Invalid caption text";

	if (this->m_lpcaption != NULL)
		free((LPVOID)this->m_lpcaption);

	INT caption_len	  =  _tcslen(lpsz_caption) + 1;
	this->m_lpcaption =  (LPTSTR)calloc(caption_len, sizeof(TCHAR));
	_tcscpy_s(this->m_lpcaption, caption_len, lpsz_caption);
}

LPCTSTR MyWindow::GetCaption() const
{
	if (this->m_lpcaption == NULL)
		return TEXT("");
	return this->m_lpcaption;
}

VOID MyWindow::SetClassName(LPCTSTR lpsz_class_name)
{
	if (lpsz_class_name == NULL && this->m_hwnd != NULL)
		throw "Invalid class name";

	if (this->m_lpclass_name != NULL)
		free((LPVOID)this->m_lpclass_name);

	INT class_name_len   =  _tcslen(lpsz_class_name) + 1;
	this->m_lpclass_name =  (LPTSTR)calloc(class_name_len, sizeof(TCHAR));
	_tcscpy_s(this->m_lpclass_name, class_name_len, lpsz_class_name);
}

LPCTSTR MyWindow::GetClassName() const
{
	if (this->m_lpclass_name == NULL)
		return TEXT("");
	return this->m_lpclass_name;
}

HWND MyWindow::Create()
{
	return this->Create(
			(HWND)NULL,
			0L,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			(HMENU)NULL,
			NULL
		);
}

HWND MyWindow::Create(HWND hwnd_parent)
{
	return this->Create(
			(HWND)hwnd_parent,
			0L,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			(HMENU)NULL,
			NULL
		);
}

HWND MyWindow::Create(HWND hwnd_parent, DWORD dw_ex_style, DWORD dw_style,
		INT x, INT y, INT width, INT height, HMENU h_menu, LPVOID lp_param)
{
	if (this->m_hwnd == NULL) 
	{
		this->m_hwnd = CreateWindowEx(
			dw_ex_style,
			this->m_lpclass_name,
			this->m_lpcaption,
			dw_style,
			x,
			y,
			width,
			height,
			(HWND)hwnd_parent,
			(HMENU)h_menu,
			this->m_hinstance,
			this
		);

		this->m_lparam = lp_param;
	}

	return this->m_hwnd;
}

BOOL MyWindow::Show(INT n_cmd_show) const
{
	if (this->m_hwnd == NULL)
		return FALSE;
	return ShowWindow(this->m_hwnd, n_cmd_show);
}

BOOL MyWindow::Show() const
{
	return this->Show(SW_SHOW);
}

LRESULT CALLBACK MyWindow::MsgRouter(HWND hwnd, UINT u_msg, WPARAM w_param,
		LPARAM l_param)
{
	if (u_msg == WM_NCCREATE) 
		SetWindowLong(hwnd, GWL_USERDATA, long(LPCREATESTRUCT(l_param)->lpCreateParams));

	LPMWND wb_window = (LPMWND)GetWindowLong(hwnd, GWL_USERDATA);

	if (wb_window)
		return wb_window->WndProc(hwnd, u_msg, w_param, l_param);

	return DefWindowProc(hwnd, u_msg, w_param, l_param);
}