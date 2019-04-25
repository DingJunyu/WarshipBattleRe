#include "FlagShipAI.h"

FlagShipAI::~FlagShipAI()
{
}

void FlagShipAI::SetWayPoint() {
	/*�����_���Ŏ��̃^�[�Q�b�g��T��*/
	RandomPoint(&wayPoint,nextPosOnMapX, nextPosOnMapZ, randRange);

	if (myPos.x > 0 && myPos.z > 0) {
		int target = rand() % 3;
		switch (target) {
		case 0:wayPoint.ChangeX(); break;//��l�ی��Ɉڂ�
		case 1:wayPoint.ChangeZ(); break;//���ی��Ɉڂ�
		case 2:wayPoint.ChangeXandZ(); break;//��O�ی��Ɉڂ�
		}
	}
}

/*�G�̐i�s�����̑O�̓_���^�[�Q�b�g�_�ɐݒ肷��*/
void FlagShipAI::SetWayPoint(Coordinate2D<double> targetPos, double radian, double speed) {
	wayPoint = targetPos;
	NextPoint(&wayPoint,radian, speed, nextPointFrame);
}

/*�ڕW�Ƃ̊p�x���v�Z����*/
void FlagShipAI::CalTargetRadian() {
	targetRadian = CalRadianBetweenPoints(wayPoint, myPos, nowRadian);
}