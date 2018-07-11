#include "stdafx.h"
#include "OleClientSideBrowserGUI.h"
#include "HelpTools.h"

using namespace ISXOleClientSideBrowserGUI;

OleClientSideBrowserGUI::OleClientSideBrowserGUI(HWND _hwnd_parent)
		: i_com_ref_count(0)
		, hwnd_parent(_hwnd_parent)
{
	::SetRect(&r_object, -300, -300, 300, 300);
	if (this->CreateBrowser() == FALSE)
	{
		return;
	}
}

bool OleClientSideBrowserGUI::CreateBrowser()
{
	HRESULT hr = ::OleCreate(CLSID_WebBrowser, IID_IOleObject,
		OLERENDER_DRAW, 0, this, this, (void**)&this->m_ole_object);

	if (FAILED(hr))
	{
		FAILURE_BOX("Can't create ole_object CLSID_WebBrowser");
		return FALSE;
	}

	hr = this->m_ole_object->SetClientSite(this);
	hr = OleSetContainedObject(this->m_ole_object, TRUE);

	RECT pos_rect;
	::SetRect(&pos_rect, -300, -300, 300, 300);
	hr = this->m_ole_object->DoVerb(OLEIVERB_INPLACEACTIVATE,
		NULL, this, -1, this->hwnd_parent, &pos_rect);

	if (FAILED(hr))
	{
		FAILURE_BOX("ole_object->DoVerb() failed");
		return FALSE;
	}

	hr = this->m_ole_object->QueryInterface(&this->m_web_browser2);

	if (FAILED(hr))
	{
		FAILURE_BOX("ole_object->QueryInterface(&web_browser2) failed");
		return FALSE;
	}

	return TRUE;
}

RECT OleClientSideBrowserGUI::PixelToHiMetric(const RECT& rect) const
{
	static BOOL s_init = false;
	static INT  s_pixels_per_inch_X;
	static INT	s_pixels_per_inch_Y;

	if (!s_init)
	{
		HDC hdc = ::GetDC(0);
		s_pixels_per_inch_X = ::GetDeviceCaps(hdc, LOGPIXELSX);
		s_pixels_per_inch_Y = ::GetDeviceCaps(hdc, LOGPIXELSY);
		::ReleaseDC(0, hdc);
		s_init = true;
	}

	RECT rc;
	rc.left = MulDiv(2540, rect.left, s_pixels_per_inch_X);
	rc.top = MulDiv(2540, rect.top, s_pixels_per_inch_Y);
	rc.right = MulDiv(2540, rect.right, s_pixels_per_inch_X);
	rc.bottom = MulDiv(2540, rect.bottom, s_pixels_per_inch_Y);
	return rc;
}

VOID OleClientSideBrowserGUI::Refresh() const
{
	this->m_web_browser2->Refresh();
}

HRESULT OleClientSideBrowserGUI::GetURL(LPBSTR lpbstr_url) const
{
	return this->m_web_browser2->get_LocationURL(lpbstr_url);
}

HRESULT OleClientSideBrowserGUI::Navigate(WSTRING sz_url) const
{
	bstr_t url(sz_url.c_str());
	variant_t flags(navNoHistory);
	return this->m_web_browser2->Navigate(url, &flags, 0, 0, 0);
}

VOID OleClientSideBrowserGUI::DoPageAction(MSG* msg) 
{
	IOleInPlaceActiveObject* pIOIPAO;
	HRESULT hr = S_FALSE;

	if (m_web_browser2)
	{
		hr = m_web_browser2->QueryInterface(IID_IOleInPlaceActiveObject, (void**)&pIOIPAO);
		if (SUCCEEDED(hr))
		{
			if (msg && msg->message == WM_KEYDOWN && msg->wParam == VK_TAB)
				msg->hwnd = m_hwnd_control;

			pIOIPAO->TranslateAccelerator(msg);
			pIOIPAO->Release();
			if (msg->wParam == VK_TAB)
			{
				msg->wParam = 0;
				msg->lParam = 0;
				msg->hwnd = nullptr;
			}
		}
	}
}

void OleClientSideBrowserGUI::SetRect(const RECT& rect)
{
	this->r_object = rect;

	RECT hi_metric_rect = this->PixelToHiMetric(this->r_object);
	SIZEL sz;
	sz.cx = hi_metric_rect.right - hi_metric_rect.left;
	sz.cy = hi_metric_rect.bottom - hi_metric_rect.top;
	this->m_ole_object->SetExtent(DVASPECT_CONTENT, &sz);

	if (this->m_ole_in_place_object != 0)
	{
		this->m_ole_in_place_object->SetObjectRects(&this->r_object,
			&this->r_object);
	}
}

// ----- IUnknown -----

HRESULT STDMETHODCALLTYPE OleClientSideBrowserGUI::QueryInterface(REFIID riid,
	void** ppv_object)
{
	if (riid == __uuidof(IUnknown))
	{
		(*ppv_object) = static_cast<IOleClientSite*>(this);
	}
	else if (riid == __uuidof(IOleInPlaceSite))
	{
		(*ppv_object) = static_cast<IOleInPlaceSite*>(this);
	}
	else
	{
		return E_NOINTERFACE;
	}

	this->AddRef();
	return S_OK;
}

ULONG STDMETHODCALLTYPE OleClientSideBrowserGUI::AddRef(void)
{
	++this->i_com_ref_count;
	return this->i_com_ref_count;
}

ULONG STDMETHODCALLTYPE OleClientSideBrowserGUI::Release(void)
{
	--this->i_com_ref_count;
	return this->i_com_ref_count;
}

// ---------- IOleWindow ----------

HRESULT STDMETHODCALLTYPE OleClientSideBrowserGUI::GetWindow(
	__RPC__deref_out_opt HWND *phwnd)
{
	(*phwnd) = this->hwnd_parent;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE OleClientSideBrowserGUI::ContextSensitiveHelp(
	BOOL fEnterMode)
{
	return E_NOTIMPL;
}

// ---------- IOleInPlaceSite ----------

HRESULT STDMETHODCALLTYPE OleClientSideBrowserGUI::CanInPlaceActivate(void)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE OleClientSideBrowserGUI::OnInPlaceActivate(void)
{
	OleLockRunning(this->m_ole_object, TRUE, FALSE);
	this->m_ole_object->QueryInterface(&this->m_ole_in_place_object);
	this->m_ole_in_place_object->SetObjectRects(&this->r_object,
		&this->r_object);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE OleClientSideBrowserGUI::OnUIActivate(void)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE OleClientSideBrowserGUI::GetWindowContext(
	__RPC__deref_out_opt IOleInPlaceFrame** pp_frame,
	__RPC__deref_out_opt IOleInPlaceUIWindow** pp_doc,
	__RPC__out LPRECT lprc_pos_rect,
	__RPC__out LPRECT lprc_clip_rect,
	__RPC__inout LPOLEINPLACEFRAMEINFO lp_frame_info)
{
	HWND hwnd = this->hwnd_parent;

	(*pp_frame) = NULL;
	(*pp_doc) = NULL;
	(*lprc_pos_rect).left = this->r_object.left;
	(*lprc_pos_rect).top = this->r_object.top;
	(*lprc_pos_rect).right = this->r_object.right;
	(*lprc_pos_rect).bottom = this->r_object.bottom;
	*lprc_clip_rect = *lprc_pos_rect;

	lp_frame_info->fMDIApp = false;
	lp_frame_info->hwndFrame = hwnd;
	lp_frame_info->haccel = NULL;
	lp_frame_info->cAccelEntries = 0;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE OleClientSideBrowserGUI::Scroll(
	SIZE scrollExtant)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE OleClientSideBrowserGUI::OnUIDeactivate(
	BOOL fUndoable)
{
	return S_OK;
}

HWND OleClientSideBrowserGUI::GetControlWindow()
{
	if (this->m_hwnd_control != 0)
		return this->m_hwnd_control;

	if (this->m_ole_in_place_object == 0)
		return 0;

	this->m_ole_in_place_object->GetWindow(&this->m_hwnd_control);
	return this->m_hwnd_control;
}

HRESULT STDMETHODCALLTYPE OleClientSideBrowserGUI::OnInPlaceDeactivate(void)
{
	this->m_hwnd_control = 0;
	this->m_ole_in_place_object = 0;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE OleClientSideBrowserGUI::DiscardUndoState(void)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE OleClientSideBrowserGUI::DeactivateAndUndo(void)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE OleClientSideBrowserGUI::OnPosRectChange(
	__RPC__in LPCRECT lprcPosRect)
{
	return E_NOTIMPL;
}

// ---------- IOleClientSite ----------

HRESULT STDMETHODCALLTYPE OleClientSideBrowserGUI::SaveObject(void)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE OleClientSideBrowserGUI::GetMoniker(
	DWORD dwAssign,
	DWORD dwWhichMoniker,
	__RPC__deref_out_opt IMoniker **ppmk)
{
	if ((dwAssign == OLEGETMONIKER_ONLYIFTHERE) &&
		(dwWhichMoniker == OLEWHICHMK_CONTAINER))
		return E_FAIL;

	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE OleClientSideBrowserGUI::GetContainer(
	__RPC__deref_out_opt IOleContainer **ppContainer)
{
	return E_NOINTERFACE;
}

HRESULT STDMETHODCALLTYPE OleClientSideBrowserGUI::ShowObject(void)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE OleClientSideBrowserGUI::OnShowWindow(
	BOOL fShow)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE OleClientSideBrowserGUI::RequestNewObjectLayout(void)
{
	return E_NOTIMPL;
}

// ----- IStorage -----

HRESULT STDMETHODCALLTYPE OleClientSideBrowserGUI::CreateStream(
	__RPC__in_string const OLECHAR *pwcsName,
	DWORD grfMode,
	DWORD reserved1,
	DWORD reserved2,
	__RPC__deref_out_opt IStream **ppstm)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE OleClientSideBrowserGUI::OpenStream(
	const OLECHAR *pwcsName,
	void *reserved1,
	DWORD grfMode,
	DWORD reserved2,
	IStream **ppstm)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE OleClientSideBrowserGUI::CreateStorage(
	__RPC__in_string const OLECHAR *pwcsName,
	DWORD grfMode,
	DWORD reserved1,
	DWORD reserved2,
	__RPC__deref_out_opt IStorage **ppstg)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE OleClientSideBrowserGUI::OpenStorage(
	__RPC__in_opt_string const OLECHAR *pwcsName,
	__RPC__in_opt IStorage *pstgPriority,
	DWORD grfMode,
	__RPC__deref_opt_in_opt SNB snbExclude,
	DWORD reserved,
	__RPC__deref_out_opt IStorage **ppstg)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE OleClientSideBrowserGUI::CopyTo(
	DWORD ciidExclude,
	const IID *rgiidExclude,
	__RPC__in_opt  SNB snbExclude,
	IStorage *pstgDest)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE OleClientSideBrowserGUI::MoveElementTo(
	__RPC__in_string const OLECHAR *pwcsName,
	__RPC__in_opt IStorage *pstgDest,
	__RPC__in_string const OLECHAR *pwcsNewName,
	DWORD grfFlags)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE OleClientSideBrowserGUI::Commit(
	DWORD grfCommitFlags)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE OleClientSideBrowserGUI::Revert(void)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE OleClientSideBrowserGUI::EnumElements(
	DWORD reserved1,
	void *reserved2,
	DWORD reserved3,
	IEnumSTATSTG **ppenum)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE OleClientSideBrowserGUI::DestroyElement(
	__RPC__in_string const OLECHAR *pwcsName)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE OleClientSideBrowserGUI::RenameElement(
	__RPC__in_string const OLECHAR *pwcsOldName,
	__RPC__in_string const OLECHAR *pwcsNewName)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE OleClientSideBrowserGUI::SetElementTimes(
	__RPC__in_opt_string const OLECHAR *pwcsName,
	__RPC__in_opt const FILETIME *pctime,
	__RPC__in_opt const FILETIME *patime,
	__RPC__in_opt const FILETIME *pmtime)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE OleClientSideBrowserGUI::SetClass(
	__RPC__in REFCLSID clsid)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE OleClientSideBrowserGUI::SetStateBits(
	DWORD grfStateBits,
	DWORD grfMask)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE OleClientSideBrowserGUI::Stat(
	__RPC__out STATSTG *pstatstg,
	DWORD grfStatFlag)
{
	return E_NOTIMPL;
}