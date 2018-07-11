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

// Aliases

typedef   signed __int8					  signed_int8_t;
typedef unsigned __int8					unsigned_int8_t;

// Useful cuts

#define LIGHT_BLUE						RGB(243, 243, 249)
#define FONT_LOGO						13
#define SB_LOGO_TEXT					TEXT("Secure Browser")
#define FONT_VERDANA					TEXT("Verdana")

#define WM_ON_AUTHWND_CLOSED			WM_USER + 100

#define SOFTSERVE_LINK					TEXT("https://softserve.academy/")
#define TEST_LINK						TEXT("http://192.168.195.180")

#define HTTP							TEXT("http")
#define HTTPS							TEXT("https")

#define	VK_F12							0x7B

// Window names

#define WINDOW_NAME						TEXT("Authentication")
#define WINDOW_CLASS_NAME				TEXT("AuthenticationWindow")
#define WB_WINDOW_NAME					TEXT("WebBrowser")
#define WB_WINDOW_CLASS_NAME			TEXT("WebBrowserWindow")

// Buttons

#define BUTTON_TAKE_PHOTO_NAME			TEXT("Take photo")
#define BUTTON_RETAKE_PHOTO_NAME		TEXT("Retake photo")
#define BUTTON_SUBMIT_NAME				TEXT("Submit")
#define BUTTON_GO						TEXT("Go")

// Errors text

#define ERROR_INIT_HOOK 				"Error to init hook"
#define ERROR_INIT_MOUSE_HOOK			"Error to init mouse hook"
#define ERROR_AUTH_STAGE				"Error to start authentication stage"
#define ERROR_START_PASS				"Error to create window of our web browser"
#define ERROR_INIT_FACE_DETECTOR		"Failed to init Face Detector"
#define EXCEPTION						"EXCEPTION: "
#define _EXCEPTION						"SOME EXCEPTION"
#define WANT_CLOSE_WB					"You really want to close browser?"
#define ERROR_MANY_PARAMS				"Too many input parameters"
#define ERROR_REMOTE_SESSION_L			"User try to pass the test in a remote session"
#define ERROR_REMOTE_SESSION_B			"You cannot pass the test in a remote session"
#define ERROR_VIRTUAL_SESSION_L			"User try to pass the test in a virtual session"
#define ERROR_VIRTUAL_SESSION_B			"You cannot pass the test in a virtual session"
#define ERROR_WRONG_LINK				"Incorrect link"
#define ERROR_WRONG_PARAM				"Incorrect input parameter, it must be a link"
#define ERROR_MIRRORING_DRIVER			"A pseudo device used to mirror application drawing for remoting or other purposes"
#define ERROR_NO_MONITORS_L				"User haven't any connected monitors"
#define ERROR_NO_MONITORS_B				"You haven't any connected monitors"
#define ERROR_FEW_MONITORS_L			"User have more than 1 connected monitors"
#define ERROR_FEW_MONITORS_B			"You have more than 1 connected monitors"
#define ERROR_REGISTER_CLASS			"Can't register a class"


// Info text

#define INFO_SUCCES_START				"All stages of the verifying are successfully passed, creating a browser window"
#define INFO_SUCCES_SESSION				"Successful user session verification"
#define INFO_CORRECT_LINK				"Successful input link verification"
#define INFO_SUCCES_MONITORS			"Successful user verification on connected monitors"