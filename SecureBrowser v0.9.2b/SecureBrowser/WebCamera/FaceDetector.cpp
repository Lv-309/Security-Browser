#include "FaceDetector.h"
#include "Tools.h"
using namespace std;

namespace ISXFaceDetector
{
FaceDetector::FaceDetector()
{
	m_detector.load(CASCADE_FILE);

	if (m_detector.empty())
	{
		cout << "empty cascade\n";
		system("pause");
	}
}

void FaceDetector::Detect()
{
	vector<cv::Rect> faces;

	while (cv::waitKey(30) != ESCAPE)
	{
		//auto current_time = chrono::high_resolution_clock::now();
		//chrono::seconds duration = chrono::duration_cast<chrono::seconds>(current_time - last_time_check);
		////cout << duration.count() << endl;
		//if (!(pause_time > duration))
		//{
		m_frame = CaptureFrame();

		m_detector.detectMultiScale(m_frame.get_frame(), faces, 1.1, 3, CV_HAAR_FIND_BIGGEST_OBJECT, cv::Size(60, 60));

		DrawRectAroundFaces(faces);

		Check(faces);

		m_frame.ShowFrame();

		/*last_time_check = current_time;
		}*/

	}
	cvDestroyWindow("Output");
}

void FaceDetector::Check(const vector<cv::Rect>& faces)
{
	static int num_of_no_face = 0;
	static int num_of_more_faces = 0;

	if (faces.size() > 1)
	{
		m_frame.SaveFrameToFile("..\\Photos\\More faces\\more faces " + to_string(++num_of_more_faces) + ".jpg");
		cout << "More faces\n";
	}
	else if (faces.size() == 0)
	{
		m_frame.SaveFrameToFile("..\\Photos\\No face\\no face " + to_string(++num_of_no_face) + ".jpg");
		cout << "No face\n";
	}
	else
	{
		cout << "I see you =)\n";
	}
}

void FaceDetector::DrawRectAroundFaces(std::vector<cv::Rect> faces)
{

	for (int i = 0; i < faces.size(); i++)
	{
		cv::Rect r = faces[i];

		cv::Point pt1(r.x + r.width, r.y + r.height);
		cv::Point pt2(r.x, r.y);

		cv::rectangle(m_frame.get_frame(), pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
	}
}
}