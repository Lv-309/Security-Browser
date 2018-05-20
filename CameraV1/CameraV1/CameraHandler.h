#pragma once

#include<escapi.h>
#include<vector>

class CameraHandler// : private Camera
{
public:
	CameraHandler()
	{
		setupESCAPI();
	};

	~CameraHandler() {};

	int GetNumOfAvailableCameras();
	std::vector<std::string> GetNamesOfAvailableCameras();
	// std::string GetNameOfChosenCamera();
private:
	bool IsAnyCameraAvailable();
};

