#include "Frame.h"

cv::Mat& Frame::GetFrame()
{
	return frame;
}

void Frame::SaveFrameToFile(std::string saving_path_and_name)
{
	cv::imwrite(saving_path_and_name, frame);
}

void Frame::ShowFrame()
{
	cv::imshow("Output", frame);
	/*if (cv::waitKey(30) >= 0)
		return;*/
}