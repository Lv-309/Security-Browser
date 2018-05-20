#include "FaceDetector.h"

using namespace std;

FaceDetector::FaceDetector()
{
	detector.load(cascade_file);
	if (detector.empty())
	{
		cout << "empty cascade\n";
		system("pause");
	}
}

void FaceDetector::Detect()
{
	vector<cv::Rect> faces;

	while (cv::waitKey(30) != 27)
	{
		frame = CaptureFrame();

		detector.detectMultiScale(frame.GetFrame(), faces, 1.1, 3, CV_HAAR_FIND_BIGGEST_OBJECT, cv::Size(60, 60));

		Check(faces);

		DrawRectAroundFaces(faces);

		frame.ShowFrame();
	}
}

void FaceDetector::Check(const vector<cv::Rect>& faces)
{
	static int num_of_no_face = 0;
	static int num_of_more_faces = 0;

	if (faces.size() > 1)
	{
		frame.SaveFrameToFile("..\\Photos\\More faces\\more faces " + to_string(++num_of_more_faces) + ".jpg");
		cout << "More faces\n";
	}
	else if (faces.size() == 0)
	{
		frame.SaveFrameToFile("..\\Photos\\No face\\no face " + to_string(++num_of_no_face) + ".jpg");
		cout << "No face\n";
	}
	else
		cout << "I see you =)\n";
}

void FaceDetector::DrawRectAroundFaces(std::vector<cv::Rect> faces)
{

	for (int i = 0; i < faces.size(); i++)
	{
		cv::Rect r = faces[i];

		cv::Point pt1(r.x + r.width, r.y + r.height);
		cv::Point pt2(r.x, r.y);

		cv::rectangle(frame.GetFrame(), pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
	}
}