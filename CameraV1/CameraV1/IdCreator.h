#pragma once

#include"Camera.h"

namespace ISXIdCreator
{
class IdCreator : protected ISXCamera::Camera
{
public:
	IdCreator() {};
	~IdCreator() {};

	void TakeIdPhoto();
	void ApproveIdPhoto();

private:
	ISXFrame::Frame m_frame;
	bool is_id_taken = false; // m_id_taken???

	enum Answer
	{
		NO, YES
	};
};
}
