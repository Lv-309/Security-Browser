#pragma once

#define FAILURE_BOX(msg)	\
		MessageBox(NULL,	\
		TEXT(msg),			\
		TEXT("Error"),		\
		MB_ICONERROR);

#define WARNING_BOX(msg)	\
		MessageBox(NULL,	\
		TEXT(msg),			\
		TEXT("Warning"),	\
		MB_ICONWARNING | MB_TOPMOST | MB_YESNO)
