#pragma once

#include"Camera.h"

class IdCreator : protected Camera
{
public:
	IdCreator() {};
	~IdCreator() {};

	void TakeIdPhoto();
	void ApproveIdPhoto();

private:
	Frame m_frame;
	bool id_taken = false; // m_id_taken???

	enum Answer
	{
		NO, YES
	};
};

