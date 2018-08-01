#pragma once

#include <opencv2\highgui.hpp>
#include "MyWindow.h"
#include "WndCtrls.h"

using ISXMyWindow::MyWindow;

namespace ISXAuthenticationWindow
{
	class AuthenticationWindow : public MyWindow
	{
	public:
		explicit AuthenticationWindow(HINSTANCE hInstance);
		void SendToMoodle();
		HWND CreateAuthWindow(HWND hParent);
		ErrorTypes GetErrorsState() const noexcept override;
	private:
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

		cv::Mat							m_frame;
		cv::VideoCapture				m_camera;
		INT								m_current_num_of_cameras;
		BOOL							is_capture_current_frame;
	};
}