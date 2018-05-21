#pragma once

#include "stdafx.h"
#include "resource.h"

#define ID_AUTH_CHILD 100

namespace ISXChildWindow
{
	class ChildWindow
	{
	public:
		ChildWindow(HWND hwnd_parent, HINSTANCE hinst_parent, LPCTSTR lpwn_class, RECT rc);
		~ChildWindow() = default;
		BOOL Show();
		ATOM RegChild();
		HWND Create(RECT client);
		static LRESULT CALLBACK ChildProc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param);
	private:
		HWND m_hwnd;
		HINSTANCE m_hinst;
		LPCTSTR m_lpwn_class;
	};
}
