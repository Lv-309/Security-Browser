#include "CameraHandler.h"

#define max_size_of_name 50

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
		char current_camera_name[max_size_of_name];
		getCaptureDeviceName(num_of_current_camera, current_camera_name, max_size_of_name);
		cameras_names.push_back(current_camera_name);
		//cout << num_of_current_camera + 1 << " camera: " << current_camera_name << endl;
		
	}
	return cameras_names;
}



//std::string CameraHandler::GetNameOfChosenCamera(){}


bool CameraHandler::IsAnyCameraAvailable()
{
	if (GetNumOfAvailableCameras() <= 0)
		return false;
	else
		return true;
}


