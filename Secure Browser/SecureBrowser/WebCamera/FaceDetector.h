#pragma once

#include<iostream>
#include<vector>
#include<chrono>

#include <opencv2\objdetect.hpp>

#include "../RestClient/Subject.h"
#include "../RestClient/RestClient.h"
#include"../TraceUtility/LogInfo.h"
#include"../TraceUtility/LogError.h"

#include"Frame.h"

namespace ISXFaceDetector
{
	class FaceDetector
	{
	public:
		FaceDetector(int num_of_chosen_camera = 0);
		~FaceDetector()
		{
			camera.release();
		};

		ISXFrame::Frame CaptureFrame();

		 void FindAnyCamera();
		void StopFaceDetector();

		//algorithms for detecting suspicious behaviour and saving photos
		//all the algorithms both save photos locally (if appropriated foldersare created) and sends to Moodle
		void SaveWithInterval(int interval_in_sec = 5);
		void SaveConstantly();

		// final algorithm
		int Run(int test_duration_in_min = 40, int max_amount_of_photos = 50, int interval_in_sec_check_suspicious_behaviour = 5);

		// auxiliary functions
		void DrawRectAroundFaces(std::vector<cv::Rect> faces);
		bool PositionChanged(int x, int width, int y, int height, int delta = 15); //delta parameter is not optimal, it should be tested


	private:
		// stop face detector if (stop == true)
		std::atomic_bool stop = false;

		cv::VideoCapture camera;
		ISXFrame::Frame frame;
		cv::CascadeClassifier detector;
		const std::string CASCADE_FILE = "haarcascade_frontalface_alt.xml";

		//face position
		std::atomic_int x0 = 0;
		std::atomic_int x1 = 0;
		std::atomic_int y0 = 0;
		std::atomic_int y1 = 0;
	};
}

