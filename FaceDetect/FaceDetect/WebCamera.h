#pragma once
#include<opencv2\highgui.hpp>
#include<iostream>

class WebCamera
{
public:
	WebCamera();
	~WebCamera();

	void InitializeCamera();
	void Capturing();
	int ChooseCamera();
private:
	cv::VideoCapture camera;
	cv::Mat frame;
};
