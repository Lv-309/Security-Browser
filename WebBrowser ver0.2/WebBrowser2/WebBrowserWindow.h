#pragma once

#include "stdafx.h"

namespace ISXWebBrowserWindow
{
	class WebBrowserWindow
		: public IOleClientSite
		, public IOleInPlaceSite
		, public IStorage
	{
	public:
		// Alias names
		typedef		std::wstring	WSTRING;
		typedef		BSTR*			LPBSTR;
		// Constructors
		WebBrowserWindow(HWND _hwnd_parent);
		~WebBrowserWindow() = default;
		// Controls 
		bool	CreateBrowser();
		void	Refresh()							const;
		void	Navigate(WSTRING sz_url)			const;
		RECT	PixelToHiMetric(const RECT& rect)	const;
		/*
		*	IUnknown interface PURE virtual method overriding
		*/
		HRESULT	STDMETHODCALLTYPE QueryInterface(
			REFIID riid,
			void** ppv_object)						override;
		ULONG	STDMETHODCALLTYPE AddRef(void)	override;
		ULONG	STDMETHODCALLTYPE Release(void)	override;
		/*
		*	IOleClientSite interface PURE virtual method overriding
		*/
		HRESULT	STDMETHODCALLTYPE GetMoniker(
			DWORD dw_assign,
			DWORD dw_moniker,
			__RPC__deref_out_opt IMoniker** ppmk)					override;
		HRESULT	STDMETHODCALLTYPE GetContainer(
			__RPC__deref_out_opt IOleContainer** pp_container)		override;
		HRESULT	STDMETHODCALLTYPE ShowObject(void)				override;
		HRESULT	STDMETHODCALLTYPE SaveObject(void)				override;
		HRESULT	STDMETHODCALLTYPE OnShowWindow(BOOL b_show)		override;
		HRESULT	STDMETHODCALLTYPE RequestNewObjectLayout(void)	override;
		/*
		*	IOleWindow interface PURE virtual method overriding
		*/
		HRESULT	STDMETHODCALLTYPE	GetWindow(
			__RPC__deref_out_opt HWND* phwnd)				override;
		HRESULT	STDMETHODCALLTYPE	ContextSensitiveHelp(
			BOOL b_enter_mode)								override;
		/*
		*	IOleInPlaceSite interface PURE virtual method overriding
		*/
		HRESULT	STDMETHODCALLTYPE	GetWindowContext(
			__RPC__deref_out_opt IOleInPlaceFrame** pp_frame,
			__RPC__deref_out_opt IOleInPlaceUIWindow** pp_doc,
			__RPC__out LPRECT lprc_pos_rect,
			__RPC__out LPRECT lprc_clip_rect,
			__RPC__inout LPOLEINPLACEFRAMEINFO lp_frame_info)		override;
		HRESULT	STDMETHODCALLTYPE	OnPosRectChange(
			__RPC__in LPCRECT lprc_pos_rect)						override;

		HRESULT	STDMETHODCALLTYPE	OnUIActivate(void)				override;
		HRESULT	STDMETHODCALLTYPE	DiscardUndoState(void)			override;
		HRESULT	STDMETHODCALLTYPE	DeactivateAndUndo(void)			override;
		HRESULT	STDMETHODCALLTYPE	OnInPlaceActivate(void)			override;
		HRESULT	STDMETHODCALLTYPE	CanInPlaceActivate(void)		override;
		HRESULT	STDMETHODCALLTYPE	OnInPlaceDeactivate(void)		override;
		HRESULT	STDMETHODCALLTYPE	Scroll(SIZE scroll_extant)		override;
		HRESULT	STDMETHODCALLTYPE	OnUIDeactivate(BOOL b_undoable)	override;

		/*
		*	IStorage interface PURE virtual method overriding
		*/
		HRESULT	STDMETHODCALLTYPE	Revert(void)			override;
		HRESULT	STDMETHODCALLTYPE	Commit(
			DWORD grfCommitFlags)							override;
		HRESULT	STDMETHODCALLTYPE	CreateStream(
			__RPC__in_string const OLECHAR* pwcs_name,
			DWORD grf_mode,
			DWORD reserved1,
			DWORD reserved2,
			__RPC__deref_out_opt IStream** ppstm)			override;
		HRESULT	STDMETHODCALLTYPE	OpenStream(
			const OLECHAR* pwcs_name,
			void* reserved1,
			DWORD grf_mode,
			DWORD reserved2,
			IStream** ppstm)								override;
		HRESULT	STDMETHODCALLTYPE	CreateStorage(
			__RPC__in_string const OLECHAR* pwcs_name,
			DWORD grf_mode,
			DWORD reserved1,
			DWORD reserved2,
			__RPC__deref_out_opt IStorage** ppstg)			override;
		HRESULT	STDMETHODCALLTYPE	OpenStorage(
			__RPC__in_opt_string const OLECHAR* pwcs_name,
			__RPC__in_opt IStorage* pstg_priority,
			DWORD grf_mode,
			__RPC__deref_opt_in_opt SNB snb_exclude,
			DWORD reserved,
			__RPC__deref_out_opt IStorage** ppstg)			override;
		HRESULT	STDMETHODCALLTYPE	CopyTo(
			DWORD ciid_exclude,
			const IID* rgiid_exclude,
			__RPC__in_opt SNB snb_exclude,
			IStorage* pstgDest)								override;
		HRESULT	STDMETHODCALLTYPE	MoveElementTo(
			__RPC__in_string const OLECHAR* pwcs_name,
			__RPC__in_opt IStorage* pstg_dest,
			__RPC__in_string const OLECHAR* pwcs_new_name,
			DWORD grf_flags)								override;
		HRESULT	STDMETHODCALLTYPE	EnumElements(
			DWORD reserved1,
			void* reserved2,
			DWORD reserved3,
			IEnumSTATSTG **ppenum) override;
		HRESULT	STDMETHODCALLTYPE	DestroyElement(
			__RPC__in_string const OLECHAR *pwcs_name)		override;
		HRESULT	STDMETHODCALLTYPE	RenameElement(
			__RPC__in_string const OLECHAR* pwcs_old_name,
			__RPC__in_string const OLECHAR* pwcs_new_name)	override;
		HRESULT	STDMETHODCALLTYPE	SetElementTimes(
			__RPC__in_opt_string const OLECHAR* pwcs_name,
			__RPC__in_opt const FILETIME* pctime,
			__RPC__in_opt const FILETIME* patime,
			__RPC__in_opt const FILETIME* pmtime)			override;
		HRESULT	STDMETHODCALLTYPE	SetClass(
			__RPC__in REFCLSID clsid)						override;
		HRESULT	STDMETHODCALLTYPE	SetStateBits(
			DWORD grf_state_bits,
			DWORD grf_mask)									override;
		HRESULT	STDMETHODCALLTYPE	Stat(
			__RPC__out STATSTG* pstatstg,
			DWORD grf_stat_flag)							override;
	// Virtual methods
	public:
		virtual HRESULT GetURL(LPBSTR lpbstr_url) const;
		virtual	VOID	SetRect(const RECT& rect);
		virtual	HWND	GetControlWindow();
	// Variables
	private:
		RECT				r_object;
		HWND				hwnd_parent;
		HWND				hwnd_control;
		LONG				i_com_ref_count;
		IOleObject*			ole_object;
		IWebBrowser2*		web_browser2;
		IOleInPlaceObject*	ole_in_place_object;
	};
}