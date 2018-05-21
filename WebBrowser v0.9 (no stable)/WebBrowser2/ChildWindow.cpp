#include "stdafx.h"
#include "ChildWindow.h"

using namespace ISXChildWindow;

ChildWindow::ChildWindow(HWND hwnd_parent, HINSTANCE hinst_parent, LPCTSTR lpwn_class, RECT rc)
	: m_hinst(hinst_parent)
	, m_hwnd(hwnd_parent)
	, m_lpwn_class(lpwn_class)
{
	this->RegChild();
	this->Create(rc);
	this->Show();
}

ATOM ChildWindow::RegChild()
{
	WNDCLASSEX wcex_child;
	wcex_child.cbSize			= sizeof(WNDCLASSEX);
	wcex_child.cbClsExtra		= 0;
	wcex_child.cbWndExtra		= 0;
	wcex_child.style			= CS_HREDRAW | CS_VREDRAW;
	wcex_child.hbrBackground	= (HBRUSH)GetStockObject(NULL_BRUSH);
	wcex_child.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wcex_child.hIcon			= LoadIcon(this->m_hinst, MAKEINTRESOURCE(IDI_WEBBROWSER2));
	wcex_child.hIconSm			= LoadIcon(this->m_hinst, MAKEINTRESOURCE(IDI_SMALL));
	wcex_child.lpszMenuName		= NULL;
	wcex_child.lpfnWndProc		= this->ChildProc;
	wcex_child.lpszClassName	= this->m_lpwn_class;
	wcex_child.hInstance		= this->m_hinst;

	if (RegisterClassEx(&wcex_child) == NULL)
		// TODO: Log error
		throw "Error";
}

HWND ChildWindow::Create(RECT rc)
{
	this->m_hwnd = CreateWindowEx(
			0,
			m_lpwn_class,
			(LPCTSTR)NULL,
			WS_CHILD | WS_BORDER | WS_VISIBLE,
			rc.left,  rc.top,
			rc.right, rc.bottom,
			this->m_hwnd,
			(HMENU)(INT)ID_AUTH_CHILD,
			this->m_hinst,
			NULL
		);
	return this->m_hwnd;
}

BOOL ChildWindow::Show()
{
	return ShowWindow(this->m_hwnd, SW_NORMAL);
}

LRESULT CALLBACK ChildWindow::ChildProc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param)
{
	if (u_msg == WM_DESTROY)
	{
		return 0;
	}
	return DefWindowProc(hwnd, u_msg, w_param, l_param);
}