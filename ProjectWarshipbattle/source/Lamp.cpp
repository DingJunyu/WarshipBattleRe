#include "Lamp.h"

Lamp::~Lamp()
{
}

void Lamp::Draw() {
	//状態によって色を設定する
	unsigned Cr = on ? colorWhenOn : colorWhenOff;
	//円を描く
	DrawCircleAA(coord.x, coord.z, radius, 8, Cr, TRUE);
}

void Lamp::SetColor(bool On, int colorNum) {
	if (On)//使っている状態の色を設定
		colorWhenOn = colorNum;
	if (!On)//使っていない状態の色を設定
		colorWhenOff = colorNum;
}