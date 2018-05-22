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
}

void IdCreator::ApproveIdPhoto()
{
	int choice;

	do
	{
		TakeIdPhoto();

		std::cout << "Do you like it? :P\n";
		std::cout << "0 - no, 1 - yes\n";
		std::cin >> choice;

	} while (choice == Answer::NO);

	cvDestroyWindow("ID_PHOTO");
	m_frame.SaveFrameToFile("..\\Photos\\ID.jpg");
}
}
