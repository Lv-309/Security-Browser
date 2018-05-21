#pragma once

#include<iostream>

#include<opencv2\highgui.hpp>

#include"Frame.h"

namespace ISXCamera
{
class Camera
{
protected:
	Camera();
	~Camera() {};

	int ChooseCamera();
	ISXFrame::Frame CaptureFrame();

private:
	cv::VideoCapture camera;
	ISXFrame::Frame frame;
};
}