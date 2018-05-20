#pragma once

#include<opencv2\highgui.hpp>
#include<iostream>

#include"Frame.h"

class Camera
{
protected:
	Camera();
	~Camera() {};

	// camera
	int ChooseCamera();
	//void TakeIdPhoto();
	Frame CaptureFrame();


private:
	cv::VideoCapture camera;
	Frame frame;

};