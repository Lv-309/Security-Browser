#include "Camera.h"

#include <iostream>

#include "CameraHandler.h"

namespace ISXCamera
{
	bool Camera::camera_is_chosen = false;
	int Camera::chosen_camera = {};

Camera::Camera()
{
	if (!camera_is_chosen)
	{
		chosen_camera = ChooseCamera();
		camera.open(chosen_camera);

		if (!camera.isOpened())
		{
			std::cout << "can't find any camera " << std::endl;
			system("pause");
			return;
		}

		camera_is_chosen = true;
	}
	else
	{
		camera.open(chosen_camera);
	}
}

int Camera::ChooseCamera()
{
	int quantity_of_cameras = 0;
	std::vector<std::string> cameras_names;
	ISXCameraHandler::CameraHandler camera_handler;

	cameras_names = camera_handler.GetNamesOfAvailableCameras();
	quantity_of_cameras = camera_handler.GetNumOfAvailableCameras();

	if (quantity_of_cameras <= 0)
		return -1;
	else if (quantity_of_cameras == 1)
		return 0;
	else
	{
		for (int num_of_current_camera(0), vector_size = (int)cameras_names.size(); num_of_current_camera < vector_size; num_of_current_camera++)
		{
			std::cout << num_of_current_camera + 1 << " camera: " << cameras_names[num_of_current_camera] << std::endl;
		}

		int chosen_camera;

		std::cout << "Please, choose camera: ";
		std::cin >> chosen_camera;

		if (chosen_camera <= 0 || chosen_camera > quantity_of_cameras) return 0; // will return the first (default) camera

		return chosen_camera - 1;
	}
}

ISXFrame::Frame Camera::CaptureFrame()
{
	camera.read(frame.get_frame());
	return frame;
}
}
