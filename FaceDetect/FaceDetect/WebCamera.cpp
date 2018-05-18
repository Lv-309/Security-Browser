#include<escapi.h>
#include "WebCamera.h"
#include "FaceDetector.h"

using namespace cv;
using namespace std;

WebCamera::WebCamera()
{
	InitializeCamera();
}

void WebCamera::InitializeCamera()
{
	camera.open(ChooseCamera());

	if (!camera.isOpened())
	{
		cout << "can't find any camera " << endl;
		system("pause");
		return;
	}
}

int WebCamera::ChooseCamera()
{
	int quantity_of_cameras = setupESCAPI();

	if (quantity_of_cameras <= 0)
		return -1;
	else if (quantity_of_cameras == 1)
		return 0;
	else 
	{
		const int size_of_name = 50;

		for (int num_of_camera(0); num_of_camera < quantity_of_cameras; num_of_camera++)
		{
			char camera_name[size_of_name];
			getCaptureDeviceName(num_of_camera, camera_name, size_of_name);
			cout << num_of_camera + 1 << " camera: " << camera_name << endl;
		}

		int choose_camera;

		cout << "Please, choose camera: ";
		cin >> choose_camera;

		if (choose_camera <= 0 || choose_camera > quantity_of_cameras)
			return 0; // will turn on the first camera

		return choose_camera - 1;
	}
}

void WebCamera::Capturing()
{
	FaceDetector detector;

	while (camera.read(frame))
	{
		if (frame.empty())
		{
			cout << "empty frame " << endl;
			system("pause");
			return;
		}

		detector.FaceDetecting(frame);

		if (waitKey(1) == 27)
			return;
	}
}

WebCamera::~WebCamera()
{
}
