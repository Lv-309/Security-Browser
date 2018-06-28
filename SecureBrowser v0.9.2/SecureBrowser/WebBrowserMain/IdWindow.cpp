#include "stdafx.h"

#include "IdWindow.h"
#include "../RestClient/Subject.h"
#include "../RestClient/RestClient.h"
#include"../TraceUtility/LogInfo.h"
#include"../TraceUtility/LogWarning.h"
#include "HelpTools.h"

#define WINDOW_NAME					TEXT("ID")
#define WINDOW_CLASS_NAME			TEXT("ID window")

#define BUTTON_TAKE_PHOTO_NAME		TEXT("Take photo")
#define BUTTON_RETAKE_PHOTO_NAME	TEXT("Retake photo")
#define BUTTON_SUBMIT_NAME			TEXT("Submit")

#define BUTTON_TAKE_PHOTO			1001
#define BUTTON_RETAKE_PHOTO			1002
#define BUTTON_SUBMIT				1003
#define COMBO_LIST_OF_CAMERAS		1004

#define SPACE_FOR_BUTTON			100
#define BUTTON_WIDTH				120
#define BUTTON_HIGTH				50

namespace ISXIdWindow
{
	IdWindow::IdWindow(HINSTANCE hInst)
		: MyWindow(WINDOW_NAME, hInst, WINDOW_CLASS_NAME)
		, is_capture_current_frame(FALSE)
	{
		while (m_camera.open(0) == FALSE)
		{
			MessageBox(NULL, L"Can't find any camera\nPlease, plug in camera", L"Warning",
				MB_OK | MB_ICONWARNING | MB_DEFAULT_DESKTOP_ONLY);

			tlf_w << AT << "Student hasn't pluged in any camera";
		}

		tlf_i << AT << "Default camera has opened succcesfully";
	}

	HWND IdWindow::CreateIdWindow(HWND hParent)
	{
		INT width = 800;
		INT hight = 700;
		INT x = (GetSystemMetrics(SM_CXSCREEN) / 2) - (width / 2);
		INT y = (GetSystemMetrics(SM_CYSCREEN) / 2) - (hight / 2);
		m_hwnd_parent = hParent;
		return CreateWindowEx(0, WINDOW_CLASS_NAME, WINDOW_NAME,
			NULL, x, y, width, hight,
			hParent, NULL, m_hinstance, this);
	}

	void IdWindow::CvMatToWinControl(const cv::Mat& img, HWND WinCtrl)
	{
		if (WinCtrl == NULL || img.empty())
			return;

		INT bits_per_point = (INT)(8 * img.elemSize());
		assert((bits_per_point == 8 || bits_per_point == 24 || bits_per_point == 32));

		//Get DC of your win control
		//CClientDC hDC(WinCtrl);

		// This is the rectangle where the control is defined
		// and where the image will appear
		RECT rect;
		GetClientRect(WinCtrl, &rect);
		//rr.top AND rr.left are always 0
		INT rectWidth = rect.right;
		INT rectHeight = rect.bottom;

		//--------------------

		INT padding = 0;
		//32 bit image is always DWORD aligned because each pixel requires 4 bytes
		if (bits_per_point < 32)
			padding = 4 - (img.cols % 4);

		if (padding == 4)
			padding = 0;

		cv::Mat temp_image;
		if (padding > 0 || img.isContinuous() == false)
		{
			// Adding needed columns on the right (max 3 px)
			cv::copyMakeBorder(img, temp_image, 0, 0, 0, padding, cv::BORDER_CONSTANT, 0);
		}
		else
		{
			temp_image = img;
		}

		//---------------------------

		uchar buffer[sizeof(BITMAPINFO) + 256 * 4];
		BITMAPINFO* bmi = (BITMAPINFO*)buffer;
		BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);
		memset(bmih, 0, sizeof(*bmih));
		bmih->biSize = sizeof(BITMAPINFOHEADER);
		bmih->biWidth = temp_image.cols;
		bmih->biHeight = -temp_image.rows;// DIB are bottom ->top
		bmih->biPlanes = 1;
		bmih->biBitCount = bits_per_point;
		bmih->biCompression = BI_RGB;

		//Sets the palette only if image is grey scale
		if (bits_per_point == 8)
		{
			RGBQUAD* palette = bmi->bmiColors;
			for (INT i = 0; i < 256; i++)
			{
				palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
				palette[i].rgbReserved = 0;
			}
		}

		//----------------------

		if (temp_image.cols == rectWidth && temp_image.rows == rectHeight)
		{
			// source and destination have same size
			// tranfer memory block
			// NOTE: the padding border will be shown here. Anyway it will be max 3px width

			SetDIBitsToDevice(GetDC(WinCtrl),
				//destination rectangle
				0, 0, rectWidth, rectHeight,
				0, 0, 0, temp_image.rows,
				temp_image.data, bmi, DIB_RGB_COLORS);
		}
		else
		{
			// Image is bigger or smaller than into destination rectangle
			// we use stretch in full rect

			// destination rectangle 
			INT destx = 0, desty = 0;
			INT destw = rectWidth;
			INT desth = rectHeight;

			// rectangle defined on source bitmap
			// using imgWidth instead of tmpImg.cols will ignore the padding border
			INT imgx = 0, imgy = 0;
			INT imgWidth = temp_image.cols - padding;
			INT imgHeight = temp_image.rows;

			StretchDIBits(GetDC(WinCtrl),
				destx, desty, destw, desth,
				imgx, imgy, imgWidth, imgHeight,
				temp_image.data, bmi, DIB_RGB_COLORS, SRCCOPY);
		}
	}

	HWND IdWindow::CreateButton(HWND hParent, int x, int y, int width, int higth, LPCWSTR button_name, HMENU hMenu)
	{
		return CreateWindowEx(0, WC_BUTTON, button_name,
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | WS_BORDER,
			x, y, width, higth,
			hParent, hMenu, m_hinstance, NULL);
	}

	HWND IdWindow::CreateListOfCameras(HWND hParent, int x, int y, int width, int higth, HMENU hMenu)
	{
		return CreateWindowEx(0, WC_COMBOBOX, NULL,
			CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_VISIBLE,
			x, y, width, higth,
			hParent, hMenu, m_hinstance, NULL);
	}

	void IdWindow::AddCameraItems()
	{
		while (m_camera_handler.GetNumOfAvailableCameras() == 0)
			MessageBox(NULL, L"Can't find any camera\nPlease, plug in camera", L"Warning",
				MB_OK | MB_ICONWARNING | MB_DEFAULT_DESKTOP_ONLY);

		m_current_num_of_cameras = m_camera_handler.GetNumOfAvailableCameras();
		std::vector<std::string> cameras = m_camera_handler.GetNamesOfAvailableCameras();


		for (const std::string& s_camera_name : cameras)
		{
			std::wstring w_camera_name(s_camera_name.begin(), s_camera_name.end());
			SendMessage(m_hwnd_list_of_cameras, (UINT)CB_ADDSTRING, NULL, (LPARAM)w_camera_name.c_str());
		}

		SendMessage(m_hwnd_list_of_cameras, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

		if (m_current_num_of_cameras == 1)
			EnableWindow(m_hwnd_list_of_cameras, FALSE);
		else if (m_current_num_of_cameras > 1 && is_capture_current_frame == FALSE)
			EnableWindow(m_hwnd_list_of_cameras, TRUE);
	}

	void  IdWindow::SendToMoodle()
	{
		std::vector<unsigned char> buffer;
		cv::imencode(".jpg", m_frame, buffer);

		Subject subj;
		RestClient client(&subj);

		//subj.setBufferData((char*)buffer.data(), buffer.size(), L"ID.jpg");

		//TO DO display msg "Your photo is being saved, wait a few seconds."
	}

	LRESULT CALLBACK IdWindow::WndProc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param)
	{
		HWND parent_window;
		switch (u_msg)
		{
		case WM_CREATE:
			//parent_window = FindWindow(TEXT("WebBrowserWindow"), NULL);
			//EnableWindow(parent_window, FALSE);

			RECT rect;

			GetClientRect(hwnd, &rect);

			m_hwnd_take_photo_button =
				CreateButton(hwnd, rect.left + 20, rect.bottom - 75, BUTTON_WIDTH, BUTTON_HIGTH,
					BUTTON_TAKE_PHOTO_NAME, (HMENU)BUTTON_TAKE_PHOTO);
			m_hwnd_retake_photo_button =
				CreateButton(hwnd, rect.left + 150, rect.bottom - 75, BUTTON_WIDTH, BUTTON_HIGTH,
					BUTTON_RETAKE_PHOTO_NAME, (HMENU)BUTTON_RETAKE_PHOTO);
			m_hwnd_submit_button =
				CreateButton(hwnd, rect.left + 280, rect.bottom - 75, BUTTON_WIDTH, BUTTON_HIGTH,
					BUTTON_SUBMIT_NAME, (HMENU)BUTTON_SUBMIT);

			m_hwnd_list_of_cameras =
				CreateListOfCameras(hwnd, rect.right - 200, rect.bottom - 75, 175, 125, (HMENU)COMBO_LIST_OF_CAMERAS);

			m_hwnd_text_choose_camera = CreateWindowEx(0, TEXT("STATIC"), TEXT("Choose camera:"),
				WS_CHILD | WS_VISIBLE,
				rect.right - 315, rect.bottom - 75, 115, 25,
				hwnd, NULL, m_hinstance, NULL);

			AddCameraItems();

			EnableWindow(m_hwnd_retake_photo_button, FALSE);
			EnableWindow(m_hwnd_submit_button, FALSE);

			m_hwnd_picture_window = CreateWindowEx(0, _T("STATIC"), NULL,
				WS_VISIBLE | WS_CHILD,
				CW_USEDEFAULT, CW_USEDEFAULT,
				rect.right, rect.bottom - SPACE_FOR_BUTTON,
				hwnd, NULL, m_hinstance, NULL);

			ShowWindow(hwnd, SW_SHOW);
			return 0;
		case WM_PAINT:
		{
			if (is_capture_current_frame == FALSE)
			{
				m_camera.read(m_frame);

				CvMatToWinControl(m_frame, m_hwnd_picture_window);

				UpdateWindow(m_hwnd_take_photo_button);
				UpdateWindow(m_hwnd_retake_photo_button);
				UpdateWindow(m_hwnd_submit_button);
				UpdateWindow(m_hwnd_list_of_cameras);
				UpdateWindow(m_hwnd_text_choose_camera);
			}
			INT cam = m_camera_handler.GetNumOfAvailableCameras();
			if (m_camera_handler.GetNumOfAvailableCameras() != m_current_num_of_cameras)
			{
				/*m_current_num_of_cameras = m_camera_handler.GetNumOfAvailableCameras();*/
				SendMessage(m_hwnd_list_of_cameras, CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
				AddCameraItems();
				m_camera.open((INT)SendMessage((HWND)l_param, (UINT)CB_GETCURSEL,
					(WPARAM)0, (LPARAM)0));
			}

			return 0; //break;
		}
		case WM_COMMAND:

			if (HIWORD(w_param) == CBN_SELCHANGE)
			{
				INT chosen_camera = (INT)SendMessage((HWND)l_param, (UINT)CB_GETCURSEL,
					(WPARAM)0, (LPARAM)0);

				if (m_camera.isOpened())
					m_camera.release();

				m_camera.open(chosen_camera);
			}

			switch (LOWORD(w_param))
			{
			case BUTTON_TAKE_PHOTO:
				is_capture_current_frame = TRUE;

				EnableWindow(m_hwnd_take_photo_button, FALSE);
				EnableWindow(m_hwnd_retake_photo_button, TRUE);
				EnableWindow(m_hwnd_submit_button, TRUE);
				EnableWindow(m_hwnd_list_of_cameras, FALSE);

				break;
			case BUTTON_RETAKE_PHOTO:
				is_capture_current_frame = FALSE;

				EnableWindow(m_hwnd_take_photo_button, TRUE);
				EnableWindow(m_hwnd_retake_photo_button, FALSE);
				EnableWindow(m_hwnd_submit_button, FALSE);

				if (m_camera_handler.GetNumOfAvailableCameras() > 1)
					EnableWindow(m_hwnd_list_of_cameras, TRUE);

				break;
			case BUTTON_SUBMIT:
				cv::imwrite("..//Photos//ID.jpg", m_frame);
				tlf_i << AT << "Student has submited his ID photo";

				SendToMoodle();

				tlf_i << AT << "User start test passing";

				//parent_window = FindWindow(TEXT("WebBrowserWindow"), NULL);
				//PostMessage(parent_window, WM_ON_IDWINDOW_CLOSED, NULL, NULL);
				PostMessage(m_hwnd_parent, WM_ON_IDWINDOW_CLOSED, NULL, NULL);
				PostMessage(hwnd, WM_CLOSE, NULL, NULL);
				break;
			}
			return 0; //break;
		case WM_MOVING:
		{
			RECT* changed = (RECT*)l_param;
			GetWindowRect(hwnd, changed);
			return 0; // break;
		}
		case WM_CLOSE:
			if (m_camera.isOpened())
				m_camera.release();

			parent_window = FindWindow(TEXT("WebBrowserWindow"), NULL);
			EnableWindow(parent_window, TRUE);
			DestroyWindow(hwnd);
			return 0;
		default:
			return DefWindowProc(hwnd, u_msg, w_param, l_param);
		}
	}
}