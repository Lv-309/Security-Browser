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

#define WM_ON_AUTHWND_CLOSED WM_USER + 100

#define WINDOW_NAME					TEXT("Authentication")
#define WINDOW_CLASS_NAME			TEXT("Authentication window")

#define BUTTON_TAKE_PHOTO_NAME		TEXT("Take photo")
#define BUTTON_RETAKE_PHOTO_NAME	TEXT("Retake photo")
#define BUTTON_SUBMIT_NAME			TEXT("Submit")