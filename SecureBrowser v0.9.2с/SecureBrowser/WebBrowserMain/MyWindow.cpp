#include "stdafx.h"
#include "MyWindow.h"
#include "MyWebBrowser.h"
#include "..\TraceUtility\LogFatal.h"

using namespace ISXMyWindow;

MyWindow::MyWindow(LPCTSTR lpsz_caption, HINSTANCE h_hinstance, LPCTSTR lpwn_class)
		: m_hinstance(h_hinstance)
{
	this->SetCaption(lpsz_caption);
	this->SetClassName(lpwn_class);
	// Class window inst
	WNDCLASSEX wcex;
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.hbrBackground	= CreateSolidBrush(LIGHT_BLUE);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon			= LoadIcon(this->m_hinstance, MAKEINTRESOURCE(IDD_WEBBROWSER2_DIALOG));
	wcex.hIconSm		= LoadIcon(this->m_hinstance, MAKEINTRESOURCE(IDI_SMALL));
	wcex.lpszMenuName	= NULL;
	// Register class
	if (this->Register(&wcex) == FALSE)
		tlf_f << AT << "Can't register a class";
}

const ATOM& MyWindow::Register(LPWNDCLASSEX wcex) const
{
	wcex->lpfnWndProc = this->MsgRouter;
	wcex->lpszClassName = this->m_class_name;
	wcex->hInstance = this->m_hinstance;

	return RegisterClassEx(wcex);
}

MyWindow::~MyWindow()
{
	if (this->m_hwnd != nullptr)
	{
		DestroyWindow(this->m_hwnd);
		this->m_hwnd = nullptr;
	}

	if (this->m_class_name != nullptr)
	{
		free((LPVOID)this->m_class_name);
		this->m_class_name = nullptr;
	}

	if (this->m_caption != nullptr)
	{
		free((LPVOID)this->m_caption);
		this->m_caption = nullptr;
	}
}

VOID MyWindow::SetCaption(LPCTSTR lpsz_caption)
{
	if (lpsz_caption == nullptr && this->m_hwnd != nullptr)
		throw "Invalid caption text";

	if (this->m_caption != nullptr)
		free((LPVOID)this->m_caption);

	size_t caption_len = _tcslen(lpsz_caption) + 1;
	this->m_caption = (LPTSTR)calloc(caption_len, sizeof(TCHAR));
	_tcscpy_s(this->m_caption, caption_len, lpsz_caption);
}

LPCTSTR MyWindow::GetCaption() const noexcept
{
	if (this->m_caption == nullptr)
		return TEXT("");
	return this->m_caption;
}

VOID MyWindow::SetClassName(LPCTSTR lpsz_class_name)
{
	if (lpsz_class_name == nullptr && this->m_hwnd != nullptr)
		throw "Invalid class name";

	if (this->m_class_name != nullptr)
		free((LPVOID)this->m_class_name);

	size_t class_name_len = _tcslen(lpsz_class_name) + 1;
	this->m_class_name = (LPTSTR)calloc(class_name_len, sizeof(TCHAR));
	_tcscpy_s(this->m_class_name, class_name_len, lpsz_class_name);
}

LPCTSTR MyWindow::GetClassName() const noexcept
{
	if (this->m_class_name == nullptr)
		return TEXT("");
	return this->m_class_name;
}

HWND MyWindow::Create()
{
	return this->Create(
			(HWND)NULL,
			0L,
			this->m_class_name,
			this->m_caption,
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
			this->m_class_name,
			this->m_caption,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			(HMENU)NULL,
			NULL
		);
}

HWND MyWindow::Create(HWND hwnd_parent, DWORD dw_ex_style, LPTSTR class_name, LPTSTR caption, DWORD dw_style,
	INT x, INT y, INT width, INT height, HMENU h_menu, LPVOID lp_param)
{
	if (this->m_hwnd == nullptr)
	{
		this->m_hwnd = CreateWindowEx(
				dw_ex_style,
				class_name,
				caption,
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

LRESULT CALLBACK MyWindow::MsgRouter(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param)
{
	MyWindow* window = nullptr;



	if (u_msg == WM_NCCREATE)
	{
		window = (MyWindow*)(LPCREATESTRUCT(l_param))->lpCreateParams;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, LONG_PTR(window));
		//window->m_hwnd = hwnd;
	}

	window = (MyWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	if (window != nullptr)
		return window->WndProc(hwnd, u_msg, w_param, l_param);

	return DefWindowProc(hwnd, u_msg, w_param, l_param);
}