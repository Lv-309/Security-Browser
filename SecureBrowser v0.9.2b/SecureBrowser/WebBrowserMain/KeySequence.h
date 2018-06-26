#pragma once
#include "stdafx.h"

namespace ISXKeySequence
{

	class KeySequence
	{
	public:
		explicit KeySequence(HINSTANCE hinst = nullptr);
		~KeySequence();

		static LRESULT CALLBACK LowLevelMouseProc(int n_code, WPARAM w_param, LPARAM l_param);
		static LRESULT CALLBACK LowLevelKeyboardProc(int n_code, WPARAM w_param, LPARAM l_param);
	private:
		HHOOK hhkLowLevelKybd  = nullptr;
		HHOOK hhkLowLevelMouse = nullptr;
	};


}