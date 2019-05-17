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

}

ErrorMessege::~ErrorMessege(){
	mes = nullptr;
}

void ErrorMessege::DrawMessege() {
	SetTrans(80);
	DrawBox(leftUp.x,leftUp.z,
		rightDown.x,rightDown.z, GetColor(231, 76, 60),TRUE);
	ResetTrans();
	DxLib::SetFontSize(20);

	DrawFormatString(leftUp.x + 5, leftUp.z + 5,
		GetColor(255, 255, 255), mes);
}