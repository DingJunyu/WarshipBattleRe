#pragma once
#include"DxLib.h"
#include"DefinedData.h"
#include"OtherFunctions.h"

class ErrorMessege
{
public:
	ErrorMessege(const char *mes, Coordinate2D<int> leftUp,
		Coordinate2D<int> rightDown);
	ErrorMessege();
	~ErrorMessege();

	void DrawMessege();
	bool ReferDestroy() { return GetTickCount() > nowTime + time; }

private:
	const unsigned int time = 1500;
	unsigned int nowTime;
	const char *mes;
	Coordinate2D<int> leftUp;
	Coordinate2D<int> rightDown;
};

