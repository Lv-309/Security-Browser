#pragma once

#include<opencv2\highgui.hpp>
#include<opencv2\imgproc.hpp>

namespace ISXFrame
{
class Frame
{
public:
	Frame() {};
	~Frame() {};

	void SaveFrameToFile(std::string saving_path_and_name) { cv::imwrite(saving_path_and_name, m_frame); }
	void ShowFrame() { cv::imshow("Output", m_frame); }

	cv::Mat& get_frame() { return m_frame; }

private:
	cv::Mat m_frame;
};
}
