#pragma once

#include<opencv2\highgui.hpp>
#include<opencv2\imgproc.hpp>

class Frame
{
public:
	Frame() {};
	~Frame() {};

	cv::Mat& GetFrame();
	void SaveFrameToFile(std::string saving_path_and_name);
	void ShowFrame();

private:
	cv::Mat frame;
};

