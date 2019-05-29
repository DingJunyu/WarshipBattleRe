#include "ColorBox.h"

ColorBox::ColorBox(){
	SetColorBox();
}

ColorBox::~ColorBox(){

}

void ColorBox::SetColorBox() {
	CrBox[GREEN] = GetColor(0, 255, 0);
	CrBox[RED] = GetColor(255, 0, 0);
	CrBox[BLUE] = GetColor(0, 0, 255);
	CrBox[BLACK] = GetColor(0, 0, 0);
	CrBox[WHITE] = GetColor(255, 255, 255);
	CrBox[MIDNIGHT] = GetColor(33, 47, 61);
	CrBox[BLUE_CHARCOAL] = GetColor(28, 40, 51);
	CrBox[GRENADIER] = GetColor(203, 67, 53);
	CrBox[LOCHMARA] = GetColor(36, 113, 163);
	CrBox[MY_SIN] = GetColor(245, 176, 65);
	CrBox[TENNE] = GetColor(211, 84, 0);
	CrBox[MEDIUM_SEA_GREEN] = GetColor(46, 204, 113);
	CrBox[TERRA_COTTA] = GetColor(236, 112, 99);
	CrBox[DROVER] = GetColor(249, 231, 159);
	CrBox[GORSE] = GetColor(255, 235, 59);
	CrBox[RED_ORANGE] = GetColor(244, 67, 54);
	CrBox[IRON_STONE] = GetColor(121, 85, 72);
	CrBox[WATER_COURSE] = GetColor(14, 102, 85);
	CrBox[ASTRONAUT_BLUE] = GetColor(21, 67, 96);
}