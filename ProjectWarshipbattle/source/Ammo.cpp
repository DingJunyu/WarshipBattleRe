#include "Ammo.h"

Ammo::~Ammo()
{
}

void Ammo::CheckAlive() {
	//�C�ɗ�������g���Ȃ��悤�ɂ���
	if (ReferCoordY() <= -3) {
		isUsable = false;
	}
}

void Ammo::SetData() {
	/*�摜�̃T�C�Y�����*/
	GetGraphSize(*ammoHandle, &graphX, &graphZ);
}

void Ammo::Draw(int x,int z){
	double realX, realZ;
	/*��������W�ƃJ�������W�����킹�Čv�Z����*/
	realX = ReferCoordX() - x;
	realZ = ReferCoordZ() - z;

	/*��ʂɓ��鎞�����`����s��*/
	if (realX > 0 && realX < Screen::SCREEN_X&&
		realZ>0 && realZ < Screen::SCREEN_Z) {
		DrawRotaGraph3((int)ReferCoordX() - x,
			(int)ReferCoordZ() - z,
			graphX / 2, graphZ / 2,
			0.08, 0.08, ReferRadianOnZ(),
			*ammoHandle, TRUE, FALSE);
	}
}