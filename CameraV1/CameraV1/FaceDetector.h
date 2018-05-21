#pragma once

#include<iostream>
#include<vector>
#include<chrono>

#include<opencv2\objdetect.hpp>

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
	cv::CascadeClassifier m_detector;
	const std::string CASCADE_FILE = "haarcascade_frontalface_alt.xml";
	Frame m_frame;
	/*std::chrono::seconds pause_time = std::chrono::seconds(3);
	std::chrono::steady_clock::time_point last_time_check = std::chrono::high_resolution_clock::time_point(std::chrono::seconds(0));*/
};

