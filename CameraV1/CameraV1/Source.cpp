#include "Camera.h"
#include "Frame.h"
#include "FaceDetector.h"
#include "IdCreator.h"


int main()
try
{
	IdCreator id;
	id.ApproveIdPhoto();

	FaceDetector detector;
	detector.Detect();

	return 0;
}
catch (const char* message)
{
	std::cerr << "Error: " << message;
	system("pause");
	return 1;
}
