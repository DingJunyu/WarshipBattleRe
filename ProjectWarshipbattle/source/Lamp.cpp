#include "Lamp.h"

Lamp::~Lamp()
{
}

void Lamp::Draw() {
	//��Ԃɂ���ĐF��ݒ肷��
	unsigned Cr = on ? colorWhenOn : colorWhenOff;
	//�~��`��
	DrawCircleAA(coord.x, coord.z, radius, 8, Cr, TRUE);
}

void Lamp::SetColor(bool On, int colorNum) {
	if (On)//�g���Ă����Ԃ̐F��ݒ�
		colorWhenOn = colorNum;
	if (!On)//�g���Ă��Ȃ���Ԃ̐F��ݒ�
		colorWhenOff = colorNum;
}