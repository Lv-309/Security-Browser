#include "FaceDetector.h"

#define ESCAPE 27 //??? "In general macros should not be used."

#define SEC std::chrono::seconds // can we do it with typedef?
#define GET_CURRENT_TIME std::chrono::high_resolution_clock::now()

namespace ISXFaceDetector
{
FaceDetector::FaceDetector(int num_of_chosen_camera)
{
	//load num_of_chosen_camera-th camera
	camera.open(num_of_chosen_camera);

	if (!camera.isOpened())
	{
		//TO DO
		//send msg "try reconnecting your camera into another port"

		std::cout << "can't find " << num_of_chosen_camera << "-th camera " << std::endl;
		system("pause");
		return;
	}

	//load cascase used for face detecting
	detector.load(CASCADE_FILE);

	if (detector.empty())
	{
		std::cout << "empty cascade\n";
		system("pause");
	}
}

ISXFrame::Frame FaceDetector::CaptureFrame()
{
	camera.read(frame.get_frame());
	return frame;
}

void FaceDetector::SaveConstantly()
{
	std::vector<cv::Rect> faces;

	while (cv::waitKey(30) != ESCAPE)
	{
		frame = CaptureFrame();
		detector.detectMultiScale(frame.get_frame(), faces, 1.1, 3, CV_HAAR_FIND_BIGGEST_OBJECT, cv::Size(60, 60));
		DrawRectAroundFaces(faces);
		SaveZeroAndManyFaces(faces);
		frame.ShowFrame();
	}
	cvDestroyWindow("Output");
}

void FaceDetector::SaveZeroAndManyFaces(const std::vector<cv::Rect>& faces)
{
	static int num_of_no_face = 0;
	static int num_of_more_faces = 0;

	if (faces.size() > 1)
	{
		frame.SaveFrameToFile("..\\Photos\\Many faces\\many faces " + std::to_string(++num_of_more_faces) + ".jpg");
		std::cout << "More faces\n";
	}
	else if (faces.size() == 0)
	{
		frame.SaveFrameToFile("..\\Photos\\No face\\no face " + std::to_string(++num_of_no_face) + ".jpg");
		std::cout << "No face\n";
	}
	else
	{
		std::cout << "I see you =)\n";
	}
}

void FaceDetector::DrawRectAroundFaces(std::vector<cv::Rect> faces)
{

	for (int i = 0; i < faces.size(); i++)
	{
		cv::Rect r = faces[i];

		cv::Point pt1(r.x + r.width, r.y + r.height);
		cv::Point pt2(r.x, r.y);

		cv::rectangle(frame.get_frame(), pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
	}
}

void FaceDetector::SaveWithInterval(int interval_in_sec)
{
	std::vector<cv::Rect> faces;

	auto current_time = GET_CURRENT_TIME;
	auto time_of_last_save = (std::chrono::steady_clock::time_point) SEC(-interval_in_sec);

	static int num_of_no_face = 0;
	static int num_of_one_face = 0;
	static int num_of_many_faces = 0;

	while (cv::waitKey(30) != ESCAPE)
	{
		current_time = GET_CURRENT_TIME;
		SEC duration = std::chrono::duration_cast<SEC>(current_time - time_of_last_save);

		if (interval_in_sec <= duration.count())
		{
			frame = CaptureFrame();
			detector.detectMultiScale(frame.get_frame(), faces, 1.1, 3, CV_HAAR_FIND_BIGGEST_OBJECT, cv::Size(60, 60));
			DrawRectAroundFaces(faces);
			frame.ShowFrame();

			if (faces.size() > 1) { frame.SaveFrameToFile("..\\Photos\\Many faces\\" + std::to_string(++num_of_many_faces) + '-' + std::to_string(faces.size()) + " faces.jpg"); }
			else if (faces.size() == 0) { frame.SaveFrameToFile("..\\Photos\\No face\\" + std::to_string(++num_of_no_face) + '-' + std::to_string(faces.size()) + " face.jpg"); }
			else { frame.SaveFrameToFile("..\\Photos\\One face\\" + std::to_string(++num_of_one_face) + '-' + std::to_string(faces.size()) + ".jpg"); }
			
			time_of_last_save = GET_CURRENT_TIME;
		}

	}
	cvDestroyWindow("Output");
}

bool FaceDetector::PositionChanged(int new_x0, int new_x1, int new_y0, int new_y1, int delta)
{
	static int x0 = 0, x1 = 0, y0 = 0, y1 = 0;
	if ((abs(new_x0 - x0) < delta) && (abs(new_x1 - x1) < delta) && (abs(new_y0 - y0) < delta) && (abs(new_y1 - y1) < delta)) return 0;
	else return 1;
}

void FaceDetector::SmartDetectAndSave(int test_duration_in_min, int max_amount_of_photos, int interval_in_sec_check_suspicious_behaviour)
{
	static int photos_num_save_with_intervals = (int) 0.3 * max_amount_of_photos + 1; // at least one photo will always be saved
	static int photos_num_save_if_suspicious = max_amount_of_photos - photos_num_save_with_intervals;
	
	int save_interval = (int) test_duration_in_min * 60 / photos_num_save_with_intervals;
	int check_interval = interval_in_sec_check_suspicious_behaviour;

	std::vector<cv::Rect> faces;

	static int num_of_no_face = 0;
	static int num_of_one_face = 0;
	static int num_of_many_faces = 0;

	bool only_one_face_was_detected = false;

	
	auto current_time = GET_CURRENT_TIME;
	auto time_of_last_save = (std::chrono::steady_clock::time_point) SEC(-check_interval);
	auto time_of_last_check = (std::chrono::steady_clock::time_point) SEC(-check_interval);

	while (cv::waitKey(30) != ESCAPE)
	{
		current_time = GET_CURRENT_TIME;
		SEC duration_after_save = std::chrono::duration_cast<SEC>(current_time - time_of_last_save);
		SEC duration_after_check = std::chrono::duration_cast<SEC>(current_time - time_of_last_check);

		//save with interval
		if (save_interval <= duration_after_save.count())
		{
			if (photos_num_save_with_intervals <= 0)
			{
				std::cout << "limit of photos saved with intervals exceeded";
				break; // need to use logger instead
			}

			frame = CaptureFrame();
			detector.detectMultiScale(frame.get_frame(), faces, 1.1, 3, CV_HAAR_FIND_BIGGEST_OBJECT, cv::Size(60, 60));
			DrawRectAroundFaces(faces);
			frame.ShowFrame();

			if ((faces.size() > 1) || (faces.size() == 0))
			{
				only_one_face_was_detected = false;
				time_of_last_save = GET_CURRENT_TIME;
				photos_num_save_with_intervals--;

				if (faces.size() == 0) { frame.SaveFrameToFile("..\\Photos\\No face\\" + std::to_string(++num_of_many_faces) + '-' + std::to_string(faces.size()) + " faces.jpg");	}
				else { frame.SaveFrameToFile("..\\Photos\\Many faces\\" + std::to_string(++num_of_many_faces) + '-' + std::to_string(faces.size()) + " faces.jpg"); }
			}
			else if (faces.size() == 1)
			{
				//if a person doesn't move and only 1 person was found on the previous frame - don't save this current frame

				cv::Rect r = faces[0];

				if ((only_one_face_was_detected == false) || PositionChanged(r.x, r.x + r.width, r.y, r.y + r.height))
				{
					frame.SaveFrameToFile("..\\Photos\\One face\\" + std::to_string(++num_of_one_face) + '-' + std::to_string(faces.size()) + ".jpg");
					time_of_last_save = GET_CURRENT_TIME;
					photos_num_save_with_intervals--;
				}
				
				only_one_face_was_detected = true;
			}
		}

		//save if suspicious
		if (check_interval <= duration_after_check.count())
		{
			if (photos_num_save_if_suspicious <= 0)
			{
				std::cout << "limit of photos saved if suspicious exceeded";
				break; // need to use logger instead
			}

			frame = CaptureFrame();
			detector.detectMultiScale(frame.get_frame(), faces, 1.1, 3, CV_HAAR_FIND_BIGGEST_OBJECT, cv::Size(60, 60));
			DrawRectAroundFaces(faces);
			frame.ShowFrame();

			if ((faces.size() > 1) || (faces.size() == 0))
			{
				only_one_face_was_detected = false;
				time_of_last_save = GET_CURRENT_TIME;
				photos_num_save_if_suspicious--;

				if (faces.size() == 0) { frame.SaveFrameToFile("..\\Photos\\No face\\" + std::to_string(++num_of_many_faces) + '-' + std::to_string(faces.size()) + " faces.jpg"); }
				else { frame.SaveFrameToFile("..\\Photos\\Many faces\\" + std::to_string(++num_of_many_faces) + '-' + std::to_string(faces.size()) + " faces.jpg"); }
			}
			else { only_one_face_was_detected = true; }

			time_of_last_check = GET_CURRENT_TIME;
		}
	}
	cvDestroyWindow("Output");
}

void FaceDetector::Run(int test_duration_in_min, int max_amount_of_photos, int interval_in_sec_check_suspicious_behaviour)
{
	static int photos_num_save_with_intervals = (int) 0.3 * max_amount_of_photos + 1; // at least one photo will always be saved
	static int photos_num_save_if_suspicious = max_amount_of_photos - photos_num_save_with_intervals;

	int save_interval = (int)test_duration_in_min * 60 / photos_num_save_with_intervals;
	int check_interval = interval_in_sec_check_suspicious_behaviour;

	std::vector<cv::Rect> faces;

	static int num_of_no_face = 0;
	static int num_of_one_face = 0;
	static int num_of_many_faces = 0;

	bool only_one_face_was_detected = false;
	
	auto current_time = GET_CURRENT_TIME;
	auto time_of_last_save = (std::chrono::steady_clock::time_point) SEC(-check_interval);
	auto time_of_last_check = (std::chrono::steady_clock::time_point) SEC(-check_interval);

	while (cv::waitKey(30) != ESCAPE)
	{
		current_time = GET_CURRENT_TIME;
		SEC duration_after_save = std::chrono::duration_cast<SEC>(current_time - time_of_last_save);
		SEC duration_after_check = std::chrono::duration_cast<SEC>(current_time - time_of_last_check);

		//save with interval
		if (save_interval <= duration_after_save.count())
		{
			if (photos_num_save_with_intervals <= 0)
			{
				std::cout << "limit of photos saved with intervals exceeded";
				break; // need to use logger instead
			}

			frame = CaptureFrame();
			detector.detectMultiScale(frame.get_frame(), faces, 1.1, 3, CV_HAAR_FIND_BIGGEST_OBJECT, cv::Size(60, 60));
			DrawRectAroundFaces(faces);

			//check if frame is valid
			//if not - camera was disconnected or another problem appeared
			if (frame.get_frame().empty())
				system("pause"); // TO DO send msg to main thread

			if ((faces.size() > 1) || (faces.size() == 0))
			{
				only_one_face_was_detected = false;
				time_of_last_save = GET_CURRENT_TIME;
				photos_num_save_with_intervals--;

				if (faces.size() == 0) { frame.SaveFrameToFile("..\\Photos\\No face\\" + std::to_string(++num_of_many_faces) + '-' + std::to_string(faces.size()) + " faces.jpg"); }
				else { frame.SaveFrameToFile("..\\Photos\\Many faces\\" + std::to_string(++num_of_many_faces) + '-' + std::to_string(faces.size()) + " faces.jpg"); }
			}
			else if (faces.size() == 1)
			{
				//if a person doesn't move and only 1 person was found on the previous frame - don't save this current frame

				cv::Rect r = faces[0];

				if ((only_one_face_was_detected == false) || PositionChanged(r.x, r.x + r.width, r.y, r.y + r.height))
				{
					frame.SaveFrameToFile("..\\Photos\\One face\\" + std::to_string(++num_of_one_face) + '-' + std::to_string(faces.size()) + ".jpg");
					time_of_last_save = GET_CURRENT_TIME;
					photos_num_save_with_intervals--;
				}

				only_one_face_was_detected = true;
			}
		}

		//save if suspicious
		if (check_interval <= duration_after_check.count())
		{
			if (photos_num_save_if_suspicious <= 0)
			{
				std::cout << "limit of photos saved if suspicious exceeded";
				break; // need to use logger instead
			}

			frame = CaptureFrame();
			detector.detectMultiScale(frame.get_frame(), faces, 1.1, 3, CV_HAAR_FIND_BIGGEST_OBJECT, cv::Size(60, 60));
			DrawRectAroundFaces(faces);

			//check if frame is valid
			//if not - camera was disconnected or another problem appeared
			if (frame.get_frame().empty())
				system("pause"); // TO DO send msg to main thread

			if ((faces.size() > 1) || (faces.size() == 0))
			{
				only_one_face_was_detected = false;
				time_of_last_save = GET_CURRENT_TIME;
				photos_num_save_if_suspicious--;

				if (faces.size() == 0) { frame.SaveFrameToFile("..\\Photos\\No face\\" + std::to_string(++num_of_many_faces) + '-' + std::to_string(faces.size()) + " faces.jpg"); }
				else { frame.SaveFrameToFile("..\\Photos\\Many faces\\" + std::to_string(++num_of_many_faces) + '-' + std::to_string(faces.size()) + " faces.jpg"); }
			}
			else { only_one_face_was_detected = true; }

			time_of_last_check = GET_CURRENT_TIME;
		}
	}
	cvDestroyWindow("Output");
}
}