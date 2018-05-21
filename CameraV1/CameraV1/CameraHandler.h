#pragma once

#include<vector>

#include<escapi.h>

namespace ISXCameraHandler
{
class CameraHandler
{
public:
	CameraHandler()
	{
		setupESCAPI();
	};
	~CameraHandler() {};

	int GetNumOfAvailableCameras();
	std::vector<std::string> GetNamesOfAvailableCameras();
	bool IsAnyCameraAvailable();
};
}