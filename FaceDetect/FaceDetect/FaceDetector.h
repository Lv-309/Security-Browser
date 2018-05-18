#pragma once
#include<opencv2\objdetect.hpp>
#include<opencv2\highgui.hpp>
#include<opencv2\imgproc.hpp>
#include<iostream>
#include<chrono>

class FaceDetector
{
public:
	FaceDetector();
	~FaceDetector();

	void FaceDetecting(const cv::Mat& frame);
	void Check(const cv::Mat& frame, const std::vector<cv::Rect> face);
	//void eye(const Mat& frame);
private:
	cv::CascadeClassifier detector;
	const std::string cascade_name = "haarcascade_frontalface_alt.xml";
	std::chrono::seconds pause_time = std::chrono::seconds(0);
	std::chrono::steady_clock::time_point last_time_check = std::chrono::high_resolution_clock::time_point(std::chrono::seconds(0));
};

