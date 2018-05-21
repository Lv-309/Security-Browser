#include "CameraHandler.h"

#define MAX_SIZE_OF_NAME 50

namespace ISXCameraHandler
{
int CameraHandler::GetNumOfAvailableCameras()
{
	return countCaptureDevices();
}

std::vector<std::string> CameraHandler::GetNamesOfAvailableCameras()
{
	int quantity_of_cameras = GetNumOfAvailableCameras();
	std::vector<std::string> cameras_names;

	if (!IsAnyCameraAvailable())
	{
		throw "no camera available";
	}

	for (int num_of_current_camera(0); num_of_current_camera < quantity_of_cameras; num_of_current_camera++)
	{
		char current_camera_name[MAX_SIZE_OF_NAME];
		getCaptureDeviceName(num_of_current_camera, current_camera_name, MAX_SIZE_OF_NAME);
		cameras_names.push_back(current_camera_name);
		//cout << num_of_current_camera + 1 << " camera: " << current_camera_name << endl;

	}
	return cameras_names;
}

bool CameraHandler::IsAnyCameraAvailable()
{
	if (GetNumOfAvailableCameras() <= 0) return false;
	else return true;
}
}
