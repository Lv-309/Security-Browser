#pragma once
#include "stdafx.h"
#include "MyWindow.h"
#include "HelpTools.h"

namespace ISXChooseCameraWindow
{
	enum class ChooseCameraControls : unsigned short
	{
		ID_BTN_OK,
		ID_LABEL,
		ID_COMBOBOX
	};

	class ChooseCameraWindow : public ISXMyWindow::MyWindow
	{
	public:
		explicit ChooseCameraWindow(HINSTANCE h_instance);
		~ChooseCameraWindow();
		const short& GetSelectedIndex() const noexcept;
		// TODO: Correct errors handle
		ErrorTypes GetErrorsState()     const noexcept override;
		VOID AddItems(const std::vector<std::wstring>& vec_items) const noexcept;
	// Callback methods
	protected:
		LRESULT CALLBACK WndProc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param) override;
	private:
		short	m_selected_index   = -1;
		HWND	m_hwnd_combo_box   = nullptr;
		bool	m_verify_selection = false;
	};
}