#pragma once
#include "stdafx.h"
#include "ErrorTypes.h"

namespace ISXKeySequence
{

	class KeySequence
	{
	public:
		explicit   KeySequence(HINSTANCE hinst = nullptr);
		ErrorTypes SetMouseHook() noexcept;
	protected:
		static LRESULT CALLBACK LowLevelMouseProc(int n_code, WPARAM w_param, LPARAM l_param);
		static LRESULT CALLBACK LowLevelKeyboardProc(int n_code, WPARAM w_param, LPARAM l_param);
	public:
		~KeySequence();
	private:
		HHOOK m_hhkLowLevelKybd  = nullptr;
		HHOOK m_hhkLowLevelMouse = nullptr;
	};


}