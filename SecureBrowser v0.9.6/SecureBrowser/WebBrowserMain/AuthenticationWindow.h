#pragma once
#include "stdafx.h"
#include "WndCtrls.h"
#include "RegisteringWindow.h"

#include "..\WebCamera\CameraHandler.h"

namespace ISXAuthenticationWindow
{
	namespace Registration = ISXRegisteringWindow;


	class AuthenticationWindow : public Registration::RegisteringWindow
	{
	public:
		explicit AuthenticationWindow(HINSTANCE hInstance);
		void AddCameraItems();
		void SendToMoodle();
		HWND CreateAuthWindow(HWND hParent);
		// TODO: Correct errors handle
		ErrorTypes GetErrorsState() const noexcept override;
	private:
		HWND CreateListOfCameras(HWND hParent, int x, int y, int width, int higth, HMENU hMenu);
		HWND CreateButton(HWND hParent, int x, int y, int width, int higth, LPCWSTR button_name, HMENU hMenu);

		LRESULT CALLBACK WndProc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param) override;
		static void CvMatToWinControl(const cv::Mat& img, HWND WinCtrl);
	public:
		~AuthenticationWindow();
	private:
		HWND							m_hwnd_submit_button;
		HWND							m_hwnd_take_photo_button;
		HWND							m_hwnd_retake_photo_button;
		HWND							m_hwnd_text_choose_camera;
		HWND							m_hwnd_list_of_cameras;
		HWND							m_hwnd_picture_window;
		//
		cv::Mat							m_frame;
		cv::VideoCapture				m_camera;
		ISXCameraHandler::CameraHandler m_camera_handler;
		INT								m_current_num_of_cameras;
		BOOL							is_capture_current_frame;
	};

}