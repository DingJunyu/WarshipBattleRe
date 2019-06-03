#include "ErrorMessege.h"

ErrorMessege::ErrorMessege(const char *Mes, Coordinate2D<int> LeftUp,
	Coordinate2D<int> RightDown)
{
	mes = Mes;//メッセージを保存する
	leftUp = LeftUp;//座標を保存する
	rightDown = RightDown;
	nowTime = GetTickCount();//生成時間を記録する
}

ErrorMessege::ErrorMessege() {

}

ErrorMessege::~ErrorMessege(){
	mes = nullptr;//メモリを解放する
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