#include "FaceDetector.h"
#include<vector>

using namespace cv;
using namespace std;

FaceDetector::FaceDetector()
{
	if (!detector.load(cascade_name))
	{
		cout << "can't upload " << cascade_name << endl;
		system("pause");
		return;
	}
}

void FaceDetector::FaceDetecting(const Mat& frame)
{
	auto current_time = chrono::high_resolution_clock::now();
	chrono::seconds duration = chrono::duration_cast<chrono::seconds>(current_time - last_time_check);
	//cout << duration.count() << endl;
	if (pause_time > duration)
		return;

	last_time_check = current_time;

	vector<Rect> rect_face;
	Mat gray_frame;
	cvtColor(frame, gray_frame, COLOR_BGR2GRAY);
	//equalizeHist(gray_frame, gray_frame);

	detector.detectMultiScale(gray_frame, rect_face, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(60, 60));// gray or color frame???

	// buffer for TCP/IP
	vector<uchar> buff;
	imencode(".jpg", frame, buff);

	Mat temp_picture = imdecode(buff, CV_LOAD_IMAGE_COLOR);

	for (int i(0); i < rect_face.size(); i++)
	{
		Point center(rect_face[i].x + rect_face[i].width / 2, rect_face[i].y + rect_face[i].height / 2);
		ellipse(frame, center, Size(rect_face[i].width / 2, rect_face[i].height / 2), 0, 0, 360, Scalar(0, 250, 0), 4, 8, 0);
	}

	Check(frame, rect_face);

	imshow("Show grey", gray_frame);
	imshow("Show", frame);
}

void FaceDetector::Check(const Mat& frame, const vector<Rect> face)
{
	ostringstream out_stream;

	if (face.size() == 0)
	{
		out_stream << "..\\Photos\\no face.jpg";
		cout << "no face\n";
		imwrite(out_stream.str(), frame);
	}
	else if (face.size() >= 2)
	{
		out_stream << "..\\Photos\\more faces.jpg";
		cout << "two faces\n";
		imwrite(out_stream.str(), frame);
	}
}

//void FaceDetector::eye(const Mat& frame)
//{
//	CascadeClassifier detector("haarcascade_profileface.xml");
//
//	if (!detector.load(cascade_name))
//	{
//		cout << "can't upload " << cascade_name << endl;
//		system("pause");
//		return;
//	}
//
//	vector<Rect> rect_face;
//	Mat gray_frame;
//
//	cvtColor(frame, gray_frame, COLOR_BGR2GRAY);
//	//equalizeHist(gray_frame, gray_frame);
//
//	detector.detectMultiScale(gray_frame, rect_face, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
//
//	for (size_t i = 0; i < rect_face.size(); i++)
//	{
//		Point center(rect_face[i].x + rect_face[i].width / 2, rect_face[i].y + rect_face[i].height / 2);
//		int radius = cvRound((rect_face[i].width + rect_face[i].height)*0.25);
//		circle(frame, center, radius, Scalar(0, 255, 0), 4, 8, 0);
//	}
//
//	imshow("eyes", frame);
//	waitKey(0);
//}

FaceDetector::~FaceDetector()
{
}
