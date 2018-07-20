#pragma once

#include<iostream>
#include<vector>
#include<chrono>
#include<experimental/filesystem>

#include <opencv2\objdetect.hpp>

#include "../RestClient/Subject.h"
#include "../RestClient/RestClient.h"
#include"../TraceUtility/LogInfo.h"
#include"../TraceUtility/LogError.h"

#include"Frame.h"


//namespace ISXFaceDetector
//{
//	class FaceDetector
//	{
//	public:
//		FaceDetector(int num_of_chosen_camera = 0); // add algorithm, path for saving
//		~FaceDetector()
//		{
//			camera.release();
//		};
//
//		ISXFrame::Frame CaptureFrame();
//
//		void RestartFaceDetector(int num_of_chosen_camera);
//		void StopFaceDetector();
//
//		//void TryFunc();
//
//		//TO DO
//		//add parameter to all detect functions
//		//save locally OR send to moddle
//
//		//algorithms for detecting suspicious behaviour and saving photos
//		//NOT FOR A SEPARATE THREAD
//		int SmartDetectAndSave(int test_duration_in_min = 40, int max_amount_of_photos = 50, int interval_in_sec_check_suspicious_behaviour = 5);
//		void SaveWithInterval(int interval_in_sec = 5);
//		void SaveConstantly();
//
//		// final algorithm for a separate thread (doesn't open an output window)
//		//FOR SEPARATE THREAD
//		int Run(int test_duration_in_min = 40, int max_amount_of_photos = 50, int interval_in_sec_check_suspicious_behaviour = 5);
//
//		// auxiliary functions
//		void DrawRectAroundFaces(std::vector<cv::Rect> faces);
//		bool PositionChanged(int x, int width, int y, int height, int delta = 15);
//
//
//	private:
//		// stop face detector if (stop == true)
//		std::atomic_bool stop = false;
//
//		cv::VideoCapture camera;
//		ISXFrame::Frame frame;
//		cv::CascadeClassifier detector;
//		const std::string CASCADE_FILE = "haarcascade_frontalface_alt.xml";
//
//		//face position
//		std::atomic_int x0 = 0;
//		std::atomic_int x1 = 0;
//		std::atomic_int y0 = 0;
//		std::atomic_int y1 = 0;
//	};
//}




namespace ISXFaceDetector
{
	class FaceDetector
	{
	public:
		FaceDetector(int num_of_chosen_camera = 0); // TO DO parameters for algorithm, moodle/local
		~FaceDetector()
		{
			camera.release();
		};

		ISXFrame::Frame CaptureFrame();

		 void FindAnyCamera();
		void StopFaceDetector();

		//TO DO
		//add parameter to all detect functions
		//save locally OR send to moddle

		//algorithms for detecting suspicious behaviour and saving photos
		//SmartDetectAndSave() is the same as Run(), so SmartDetectAndSave() can be deleted
		int SmartDetectAndSave(int test_duration_in_min = 40, int max_amount_of_photos = 50, int interval_in_sec_check_suspicious_behaviour = 5);
		void SaveWithInterval(int interval_in_sec = 5);
		void SaveConstantly();

		// final algorithm for a separate thread (doesn't open an output window)
		int Run(int test_duration_in_min = 40, int max_amount_of_photos = 50, int interval_in_sec_check_suspicious_behaviour = 5);

		// auxiliary functions
		void DrawRectAroundFaces(std::vector<cv::Rect> faces);
		bool PositionChanged(int x, int width, int y, int height, int delta = 15);


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

