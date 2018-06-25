#pragma once

#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>

namespace ISXFrame
{
class Frame
{
public:
	Frame() {};
	~Frame() {};

	void SaveFrameToFile(std::string saving_path_and_name) { cv::imwrite(saving_path_and_name,  frame); }

	void ShowFrame()
	{
		if (frame.empty()) system("pause");
		else cv::imshow("Output",  frame);
	}

	bool IsFrameEmpty() //function used for unit testing only
	{
		if (frame.empty())
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	cv::Mat& get_frame() { return  frame; }

	Frame& operator = (const cv::Mat& new_frame)
	{
		this->frame = new_frame;
		return *this;
	}

	Frame& operator = (const Frame& new_frame)
	{
		this->frame = new_frame.frame;
		return *this;
	}

private:
	cv::Mat frame;
};
}
