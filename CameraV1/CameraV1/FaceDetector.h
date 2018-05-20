#pragma once

#include<opencv2\objdetect.hpp>
#include<vector>
#include<iostream>

#include"Camera.h"

class FaceDetector : protected Camera
{
public:
	FaceDetector();
	~FaceDetector() {};

	void Detect();
	void Check(const std::vector<cv::Rect>& faces);
	void DrawRectAroundFaces(std::vector<cv::Rect> faces);
private:
	cv::CascadeClassifier detector;
	const std::string cascade_file = "haarcascade_frontalface_alt.xml";
	Frame frame;
};

