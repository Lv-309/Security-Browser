#include "IdCreator.h"

#define ENTER 13 //??? "In general macros should not be used."

namespace ISXIdCreator
{
void IdCreator::TakeIdPhoto()
{
	while (!is_id_taken)
	{
		m_frame = CaptureFrame();
		imshow("ID_PHOTO", m_frame.get_frame());

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

		cv::imshow("YOUR_ID_PHOTO", m_frame.get_frame());
		cv::waitKey(1);

		std::cout << "Do you like it? :P\n";
		std::cout << "0 - no, 1 - yes\n";
		std::cin >> choice;

		cvDestroyWindow("YOUR_ID_PHOTO");

	} while (choice == Answer::NO);

	m_frame.SaveFrameToFile("..\\Photos\\ID.jpg");
}
}
