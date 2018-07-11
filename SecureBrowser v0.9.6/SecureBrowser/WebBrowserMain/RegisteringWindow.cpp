#include "stdafx.h"
#include "RegisteringWindow.h"
#include "..\TraceUtility\LogFatal.h"

using namespace ISXRegisteringWindow;

RegisteringWindow::RegisteringWindow(LPCTSTR lpsz_caption, HINSTANCE h_hinstance, LPCTSTR lpwn_class)
		: m_hinstance(h_hinstance)
		, m_error_state(ErrorTypes::NULL_STATE)
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
		tlf_f << AT << ERROR_REGISTER_CLASS;
}

const ATOM RegisteringWindow::Register(LPWNDCLASSEX wcex) const
{
	wcex->lpfnWndProc   = this->MsgRouter;
	wcex->lpszClassName = this->m_class_name;
	wcex->hInstance     = this->m_hinstance;

	return RegisterClassEx(wcex);
}

RegisteringWindow::~RegisteringWindow()
{
	UnregisterClass(this->m_class_name, this->m_hinstance);

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

VOID RegisteringWindow::SetCaption(LPCTSTR lpsz_caption)
{
	if (lpsz_caption == nullptr && this->m_hwnd != nullptr)
		this->m_error_state = ErrorTypes::INVALID_CAPTION;

	if (this->m_caption != nullptr)
		free((LPVOID)this->m_caption);

	size_t caption_len = _tcslen(lpsz_caption) + 1;
	this->m_caption = (LPTSTR)calloc(caption_len, sizeof(TCHAR));
	_tcscpy_s(this->m_caption, caption_len, lpsz_caption);
}

LPCTSTR RegisteringWindow::GetCaption() const noexcept
{
	if (this->m_caption == nullptr)
		return TEXT("");
	return this->m_caption;
}

VOID RegisteringWindow::SetClassName(LPCTSTR lpsz_class_name)
{
	if (lpsz_class_name == nullptr && this->m_hwnd != nullptr)
		this->m_error_state = ErrorTypes::INVALID_CLASS_NAME;

	if (this->m_class_name != nullptr)
		free((LPVOID)this->m_class_name);

	size_t class_name_len = _tcslen(lpsz_class_name) + 1;
	this->m_class_name = (LPTSTR)calloc(class_name_len, sizeof(TCHAR));
	_tcscpy_s(this->m_class_name, class_name_len, lpsz_class_name);
}

LPCTSTR RegisteringWindow::GetClassName() const noexcept
{
	if (this->m_class_name == nullptr)
		return TEXT("");
	return this->m_class_name;
}

HWND RegisteringWindow::Create()
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

HWND RegisteringWindow::Create(HWND hwnd_parent)
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

HWND RegisteringWindow::Create(HWND hwnd_parent, DWORD dw_ex_style, LPTSTR class_name, LPTSTR caption, DWORD dw_style,
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

LRESULT CALLBACK RegisteringWindow::MsgRouter(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param)
{
	RegisteringWindow* window = nullptr;

	if (u_msg == WM_NCCREATE)
	{
		window = (RegisteringWindow*)(LPCREATESTRUCT(l_param))->lpCreateParams;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, LONG_PTR(window));
	}

	window = (RegisteringWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	if (window != nullptr)
		return window->WndProc(hwnd, u_msg, w_param, l_param);

	return DefWindowProc(hwnd, u_msg, w_param, l_param);
}