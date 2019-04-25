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
void FlagShipAI::CalTargetRadianAndSetRadianNeeded() {
	using namespace MathAndPhysics;

	targetRadian = CalRadianBetweenPoints(wayPoint, myPos, nowRadian);

	if (targetRadian > 60 * OneDegreeRadian) {
		radianNeededNow = 15 * OneDegreeRadian;
	}
	if (targetRadian <= 60 * OneDegreeRadian&&
		targetRadian > 30 * OneDegreeRadian) {
		radianNeededNow = 9 * OneDegreeRadian;
	}
	if (targetRadian <= 30 * OneDegreeRadian&&
		targetRadian > 10 * OneDegreeRadian) {
		radianNeededNow = 4 * OneDegreeRadian;
	}
	if (targetRadian <= 10 * OneDegreeRadian&&
		targetRadian > 0) {
		radianNeededNow = OneDegreeRadian;
	}
	if (targetRadian == 0) {
		radianNeededNow = 0;
	}
	if (targetRadian < 0 &&
		targetRadian >= -10 * OneDegreeRadian) {
		radianNeededNow = -OneDegreeRadian;
	}
}

