#pragma once

#include <vector>
#include "escapi.h"

namespace ISXCameraHandler
{
const int MAX_SIZE_OF_NAME = 50;

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