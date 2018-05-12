#pragma once
#include "resource.h"

namespace ISXMyWindow
{
	class MyWindow
	{
	public:
		typedef MyWindow* LPMWND;

		explicit MyWindow(LPCTSTR lpsz_caption, LPWNDCLASSEX lpwn_class_ex);
		explicit MyWindow(LPCTSTR lpsz_caption, HINSTANCE h_hinstance, LPCTSTR lpwn_class);
	#ifdef _INIT_CUSTOM_WND
		explicit MyWindow(LPCTSTR lpsz_caption, HINSTANCE h_hinstance, LPCTSTR lpwn_class,
				UINT style, HICON h_icon, HCURSOR h_cursor, HBRUSH hbr_background,
				LPCTSTR lpsz_menu_name, HICON h_icon_sm);
	#endif
		virtual ~MyWindow();
	// Disable undesirable methods
	private:
		MyWindow()							 = delete;
		MyWindow(const MyWindow&)			 = delete;
		MyWindow& operator=(const MyWindow&) = delete;
	// Public getters funcs
	public:
		virtual LPCTSTR GetCaption()   const;
		virtual LPCTSTR GetClassName() const;
		// TODO: Maybe create getter to get HWND
	// Creating window controls
	public:
		virtual VOID SetCaption(LPCTSTR lpsz_caption);
		virtual void Setwparam(LPVOID param);
		virtual LPVOID Getwparam() const;
		virtual BOOL Show()				  const;
		virtual BOOL Show(INT n_cmd_show) const;

		virtual HWND Create();
		virtual HWND Create(HWND hwnd_parent);
		virtual HWND Create(HWND hwnd_parent, DWORD dw_ex_style, DWORD dw_style,
				INT x, INT y, INT width, INT height, HMENU h_menu, LPVOID lp_param);
	// Static callback method, allowing us to pass it 
	// as the lpfnWndProc member in WNDCLASSEX
	public:
		static  LRESULT CALLBACK MsgRouter(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param);
	// Save methods
	protected:
		virtual ATOM Register(LPWNDCLASSEX wcex);
		virtual VOID SetClassName(LPCTSTR lpsz_class_name);
		// Getting abstract
		virtual LRESULT CALLBACK WndProc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param) = 0;
	// Variables for describing window
	//private:
		HWND		m_hwnd				= NULL;
		HINSTANCE	m_hinstance			= NULL;
		LPTSTR		m_lpclass_name		= NULL;
		LPTSTR		m_lpcaption			= NULL;
		LPVOID		m_wparam			= NULL;
		LPVOID		m_lparam			= NULL;
		WNDPROC		m_wndproc_default	= NULL;
	};
}