#include "stdafx.h"
#include "ChooseCameraWindow.h"

using namespace ISXChooseCameraWindow;

ChooseCameraWindow::ChooseCameraWindow(HINSTANCE h_instance)
	: MyWindow(TEXT("Choose Camera"), h_instance, TEXT("ChooseCameraWindow"))
{ }

ChooseCameraWindow::~ChooseCameraWindow()
{ }

const short& ChooseCameraWindow::GetSelectedIndex() const noexcept
{
	return this->m_selected_index;
}

VOID ChooseCameraWindow::AddItems(const std::vector<std::wstring>& vec_items) const noexcept
{
	for (const auto& item : vec_items)
		SendMessage(this->m_hwnd_combo_box, CB_ADDSTRING, NULL, (LPARAM)item.c_str());
}

LRESULT CALLBACK ChooseCameraWindow::WndProc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param)
{
	LONG		style;
	INT 		wm_id, wm_event;
	DWORD		label_id;
	RECT		rc_client;
	HWND		parent;

	switch (u_msg)
	{
	// Create window
	case WM_CREATE:
		parent = FindWindow(TEXT("WebBrowserWindow"), NULL);
		PostMessage(parent, EnableWindow(parent, FALSE), NULL, NULL);
		EnableMenuItem(GetSystemMenu(hwnd, FALSE), SC_CLOSE, MF_DISABLED | MF_GRAYED);
		// Creating Label
		CreateWindow(TEXT("STATIC"), TEXT("Choose your web camera:"), WS_CHILD | WS_VISIBLE | WS_TABSTOP,
			45, 15, 300, 20, hwnd, (HMENU)ChooseCameraControls::ID_LABEL, this->m_hinstance, NULL);
		// Creating Button
		CreateWindow(TEXT("BUTTON"), TEXT("OK"), WS_CHILD | WS_VISIBLE | LBS_NOTIFY,
			245, 90, 100, 25, hwnd, (HMENU)ChooseCameraControls::ID_BTN_OK, this->m_hinstance, NULL);
		// Creating ComboBox
		this->m_hwnd_combo_box = CreateWindow(WC_COMBOBOX, NULL, CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_BORDER | WS_VISIBLE | LBS_NOTIFY,
			45, 50, 300, 200, hwnd, (HMENU)ChooseCameraControls::ID_COMBOBOX, this->m_hinstance, NULL);
		// Set window longon
		GetClientRect(hwnd, &rc_client);
		SetWindowPos(hwnd, HWND_TOPMOST, (rc_client.right / 2), (rc_client.bottom / 2) + 100, 400, 180, NULL);
		style = GetWindowLong(hwnd, GWL_STYLE);
		style &= ~WS_MAXIMIZEBOX;
		style &= ~WS_MINIMIZEBOX;
		style &= ~WS_THICKFRAME;
		SetWindowLongPtr(hwnd, GWL_STYLE, style);
		ShowWindow(hwnd, SW_NORMAL);
		return 0;
	case WM_CTLCOLORSTATIC:
		label_id = GetDlgCtrlID((HWND)l_param);
		if (label_id == (INT)ChooseCameraControls::ID_LABEL)
		{
			SetBkColor((HDC)w_param, LIGHT_BLUE);
			return (INT_PTR)CreateSolidBrush(LIGHT_BLUE);
		}
		return 0;
	case WM_COMMAND:
		wm_id    = LOWORD(w_param);
		wm_event = HIWORD(w_param);
		if (wm_event == LBN_SELCHANGE)
		{
			this->m_verify_selection = true;
			return 0;
		}
		if (wm_id == (INT)ChooseCameraControls::ID_BTN_OK)
		{
			if (this->m_verify_selection == true)
			{
				this->m_selected_index = ComboBox_GetCurSel(this->m_hwnd_combo_box);
				PostMessage(hwnd, WM_CLOSE, NULL, NULL);
			}
			else
				WARNING_BOX("Choose your camera!");
			return 0;
		}
		return 0;
	case WM_CLOSE:
		parent = FindWindow(TEXT("WebBrowserWindow"), NULL);
		PostMessage(parent, EnableWindow(parent, TRUE), NULL, NULL);
		DestroyWindow(hwnd);
		return 0;
	// case WM_DESTROY:
	default:
		return DefWindowProc(hwnd, u_msg, w_param, l_param);
	}

	return 0;
}