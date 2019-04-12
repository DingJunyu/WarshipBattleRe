#include "Lamp.h"

Lamp::~Lamp()
{
}

void Lamp::Draw() {
	unsigned Cr = on ? colorWhenOn : colorWhenOff;

	DrawCircleAA(coord.x, coord.z, radius, 8, Cr, TRUE);
}

void Lamp::SetColor(bool On, int colorNum) {
	if (On)
		colorWhenOn = colorNum;
	if (!On)
		colorWhenOff = colorNum;
}