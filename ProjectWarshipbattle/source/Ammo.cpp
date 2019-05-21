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

void Ammo::Draw(Camera CM){
	double realX, realZ;
	/*��������W�ƃJ�������W�����킹�Čv�Z����*/
	realX = ReferCoordX() - CM.ReferRealCameraX();
	realZ = ReferCoordZ() - CM.ReferRealCameraZ();
	realX *= CM.ReferZoomRatio();
	realZ *= CM.ReferZoomRatio();

	/*��ʂɓ��鎞�����`����s��*/
	if (realX > 0 && realX < Screen::SCREEN_X&&
		realZ>0 && realZ < Screen::SCREEN_Z) {
		DrawRotaGraph3((int)realX,
			(int)realZ,
			graphX / 2, graphZ / 2,
			multipleRate * CM.ReferZoomRatio(),
			multipleRate * CM.ReferZoomRatio(),
			ReferRadianOnZ(),
			*ammoHandle, TRUE, FALSE);
	}
}