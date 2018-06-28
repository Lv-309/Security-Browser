#pragma once

#define FAILURE_BOX(msg)	\
		MessageBox(NULL,	\
		TEXT(msg),			\
		TEXT("Error"),		\
		MB_ICONERROR | MB_TOPMOST);

#define WARNING_BOX(msg)	\
		MessageBox(NULL,	\
		TEXT(msg),			\
		TEXT("Warning"),	\
		MB_ICONWARNING | MB_TOPMOST | MB_YESNO)


#define LIGHT_BLUE	RGB(243, 243, 249)
#define FONT_LOGO	13
#define VK_KEY_C 0x43
#define IS_PRESSED 0x8000
#define WM_ON_IDWINDOW_CLOSED WM_USER+100