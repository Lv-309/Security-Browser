#include "IdCreator.h"

#define ENTER 13

void IdCreator::TakeIdPhoto()
{
	while (!id_taken)
	{
		frame = CaptureFrame();
		imshow("ID_PHOTO", frame.GetFrame());

		if (cv::waitKey(30) == ENTER)
			break;
	}

	cvDestroyWindow("ID_PHOTO");
}

void IdCreator::ApproveIdPhoto()
{
	int choice;

	do
	{
		TakeIdPhoto();

		cv::imshow("YOUR_ID_PHOTO", frame.GetFrame());
		cv::waitKey(1);		

		std::cout << "Do you like it? :P\n";
		std::cout << "0 - no, 1 - yes\n";
		std::cin >> choice;
	
		cvDestroyWindow("YOUR_ID_PHOTO");

	} while (choice == Answer::NO);

	frame.SaveFrameToFile("..\\Photos\\ID.jpg");
}
