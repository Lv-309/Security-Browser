#pragma once
#include "resource.h"
#include "ErrorTypes.h"

namespace ISXMyWindow
{

	class MyWindow
	{
	// Ctors description
	public:
		explicit MyWindow(LPCTSTR lpsz_caption, HINSTANCE h_hinstance, LPCTSTR lpwn_class);
	// Disabling undesirable constructions
	private:
		MyWindow(const MyWindow& other) = delete;
		MyWindow(const MyWindow&& move) = delete;
		MyWindow& operator=(const MyWindow& other) = delete;
	// Dtor description
	protected:
		virtual ~MyWindow();
	// Public getters
	public:
		virtual LPCTSTR GetCaption()   const noexcept;
		virtual LPCTSTR GetClassName() const noexcept;
	// Public setters
	public:
		virtual VOID SetCaption(LPCTSTR lpsz_caption);
		virtual VOID SetClassName(LPCTSTR lpsz_class_name);
		virtual VOID SetFont(HWND handle, LPCTSTR font_name, SHORT font_size, LONG font_weight) const noexcept;
	// Creating window controls
	public:
		virtual HWND Create();
		virtual HWND Create(HWND hwnd_parent);
		virtual HWND Create(HWND hwnd_parent, DWORD dw_ex_style, DWORD dw_style,
				INT x, INT y, INT width, INT height, HMENU h_menu, LPVOID lp_param);
	// Inherits
	protected:
		const ATOM& Register(LPWNDCLASSEX wcex) const;
		// TODO: commend 
		virtual LRESULT CALLBACK WndProc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param) PURE;
		// Static callback method, allowing us to pass it 
		// , as the lpfnWndProc member in WNDCLASSEX
		static  LRESULT CALLBACK MsgRouter(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param);
	// Variables for describing window
	private:
		LPVOID		m_lparam        = nullptr;
		LPTSTR		m_caption       = nullptr;
		LPTSTR		m_class_name    = nullptr;
	protected:
		HWND		m_hwnd          = nullptr;
		HINSTANCE	m_hinstance;
	};

}