#pragma once

#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>

#include "../RestClient/Subject.h"
#include "../RestClient/RestClient.h"
#include "../TraceUtility/LogError.h"


namespace ISXFrame
{
	class Frame
	{
	public:
		Frame() {};
		~Frame() {};

		void SaveFrameToFile(std::string saving_path_and_name)
		{
			if (frame.empty()) tlf_e << AT << "Frame not valid";
			else cv::imwrite(saving_path_and_name, frame);
		}

		void SendToMoodle(std::wstring photo_name)
		{
			if (frame.empty()) tlf_e << AT << "Frame not valid";
			else
			{
				std::vector<unsigned char> buffer;

				buffer.resize(100 * 1024);
				cv::imencode(".jpg", frame, buffer);

				Subject subj;
				RestClient client(&subj);
				// TODO: write logs

				subj.setBufferData((char*)buffer.data(), buffer.size(), photo_name);
			}
		}

		void ShowFrame()
		{
			if (frame.empty()) tlf_e << AT << "Frame not valid";
			else cv::imshow("Output", frame);
		}

		bool IsFrameEmpty() //function not used, can be deleted
		{
			if (frame.empty())
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		cv::Mat& get_frame()
		{
			tlf_e << AT << "Frame not valid";
			return  frame;
		}

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
