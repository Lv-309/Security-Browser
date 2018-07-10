#pragma once

#include <iostream>
#include <opencv2\highgui.hpp>
#include "Frame.h"

namespace ISXCamera
{

class Camera
{
protected:
	Camera();
	~Camera() = default;

	int ChooseCamera();
	ISXFrame::Frame CaptureFrame();

private:
	cv::VideoCapture camera;
	ISXFrame::Frame frame;
	static int chosen_camera;
	static bool camera_is_chosen;
};

}