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

/*�G�Ƌ߂Â��ƓG�̐i�s�����Ɠ��������ɐi��*/
void FlagShipAI::DisableWayPoint_MoveWithEnemy(double radian) {
	targetRadian = radian;
}

/*�ڕW�Ƃ̊p�x���v�Z����*/
void FlagShipAI::CalTargetRadianAndSetRadianNeeded() {
	using namespace MathAndPhysics;

	targetRadian = CalRadianBetweenPoints(wayPoint, myPos, nowRadian);

	/*�͈͂��獡�K�v�Ȋp�x���v�Z����*/
	if (abs(targetRadian) > RANGE_MAX * OneDegreeRadian) {
		radianNeededNow = SPEED_MAX * OneDegreeRadian;
	}
	if (abs(targetRadian) <= RANGE_MAX * OneDegreeRadian&&
		abs(targetRadian) > RANGE_1_2 * OneDegreeRadian) {
		radianNeededNow = SPEED_1_2 * OneDegreeRadian;
	}
	if (abs(targetRadian) <= RANGE_1_2 * OneDegreeRadian&&
		abs(targetRadian) > RANGE_1_4 * OneDegreeRadian) {
		radianNeededNow = SPEED_1_4 * OneDegreeRadian;
	}
	if (abs(targetRadian) <= RANGE_1_4 * OneDegreeRadian&&
		abs(targetRadian) > RANGE_1_8) {
		radianNeededNow = SPEED_1_8 * OneDegreeRadian;
	}
	if (targetRadian == RANGE_1_8) {
		radianNeededNow = 0;
	}

	/*�������C������*/
	if (targetRadian < 0)
		radianNeededNow = -radianNeededNow;
}

/*�������v�Z����֐�*/
void FlagShipAI::CalDistance() {
	distance = Distance2D(wayPoint, myPos);
}
