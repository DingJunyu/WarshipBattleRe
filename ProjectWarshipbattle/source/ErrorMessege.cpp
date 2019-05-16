#include "ErrorMessege.h"

ErrorMessege::ErrorMessege(const char *Mes, Coordinate2D<int> LeftUp,
	Coordinate2D<int> RightDown)
{
	mes = Mes;
	leftUp = LeftUp;
	rightDown = RightDown;
	nowTime = GetTickCount();
}

ErrorMessege::ErrorMessege() {
	mes = nullptr;
}

ErrorMessege::~ErrorMessege()
{
}

void ErrorMessege::DrawMessege() {
	DrawBox(leftUp.x,leftUp.z,
		rightDown.x,rightDown.z, GetColor(52,73,94),TRUE);

	DxLib::SetFontSize(20);

	DrawFormatString(leftUp.x, leftUp.z,
		GetColor(255, 255, 255), mes);
}