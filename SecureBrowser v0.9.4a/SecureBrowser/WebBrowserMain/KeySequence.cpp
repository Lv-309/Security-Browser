#include "KeySequence.h"

using namespace ISXKeySequence;

KeySequence::KeySequence(HINSTANCE hinst /* = nullptr */)
{
	this->hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, this->LowLevelKeyboardProc, hinst, 0);
}

KeySequence::~KeySequence()
{
	if (this->hhkLowLevelKybd != nullptr)
		UnhookWindowsHookEx(this->hhkLowLevelKybd);
	if (this->hhkLowLevelMouse != nullptr)
		UnhookWindowsHookEx(this->hhkLowLevelMouse);
}

ErrorTypes KeySequence::SetMouseHook() noexcept
{
	this->hhkLowLevelKybd = SetWindowsHookEx(WH_MOUSE_LL, this->LowLevelMouseProc, NULL, 0);
	if (this->hhkLowLevelKybd == nullptr)
		return ErrorTypes::SET_HOOK_ERROR;
	return ErrorTypes::IS_OK;
}

LRESULT CALLBACK KeySequence::LowLevelMouseProc(int n_code, WPARAM w_param, LPARAM l_param)
{
	if (n_code == HC_ACTION)
	{
		MSLLHOOKSTRUCT &msll = *(reinterpret_cast<MSLLHOOKSTRUCT*>(l_param));
		if (w_param == WM_RBUTTONDOWN)
			return 1; // This will make the click be ignored
		return 0;
	}
	return CallNextHookEx(NULL, n_code, w_param, l_param);
}

LRESULT CALLBACK KeySequence::LowLevelKeyboardProc(int n_code, WPARAM w_param, LPARAM l_param)
{
	BOOL b_eat_keystroke = FALSE;
	if (n_code == HC_ACTION)
	{
		switch (w_param)
		{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)l_param;
			BOOL b_control_keydown = GetAsyncKeyState(VK_CONTROL);
			BOOL b_shift_keydown = GetAsyncKeyState(VK_SHIFT);

			b_eat_keystroke = (p->vkCode == VK_TAB && (p->flags & LLKHF_ALTDOWN) ||
				b_control_keydown && p->vkCode == 0x43 || b_control_keydown && p->vkCode == VK_ESCAPE ||
				p->vkCode == (VK_F12 | VK_SNAPSHOT | VK_LWIN | VK_RWIN) ||
				b_control_keydown && b_shift_keydown && p->vkCode == VK_ESCAPE);
			break;
		}
	}
	return (b_eat_keystroke ? 1 : CallNextHookEx(NULL, n_code, w_param, l_param));
}