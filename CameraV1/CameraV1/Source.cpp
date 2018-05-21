#include "Camera.h"
#include "Frame.h"
#include "FaceDetector.h"
#include "IdCreator.h"

int main()
try
{
	ISXIdCreator::IdCreator id;
	id.ApproveIdPhoto();

	ISXFaceDetector::FaceDetector detector;
	detector.Detect();

	return 0;
}
catch (const char* message)
{
	std::cerr << "Error: " << message << std::endl;
	system("pause");
	return 1;
}
