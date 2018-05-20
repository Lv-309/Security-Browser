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
	Frame frame;
	bool id_taken = false;
	//bool id_approved = false;

	enum Answer
	{
		NO, YES
	};
};

