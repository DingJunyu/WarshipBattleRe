#include "FlagShipAI.h"

FlagShipAI::~FlagShipAI()
{
}

void FlagShipAI::LetUsGo(ShipMain *me, ShipMain *target) {
	counter %= nextCounter;
	counter++;

	SetMyPos(me->ReferCoord2D_d());//�����̈ʒu��ݒ肷��
	SetNowRadian(me->ReferRadianOnZ());//�����̊p�x��ݒ肷��
	targetDistance = Distance2D(target->ReferCoord2D_d(), myPos);//�ڕW�Ƃ̋����𑪗ʂ���
	CalDistance();//�O�̃E�F�C�|�C���g�Ƃ̋������v�Z����
	if (targetDistance > (double)DistanceRange::BATTLE) {
		if (targetDistance > (double)DistanceRange::PATROL_RANGE) {
			if (distance < needToChange)//�O�̃E�F�C�|�C���g���g���Ȃ��Ȃ鎞��
				SetWayPoint();//�V�����E�F�C�|�C���g�����		
		}
		if (targetDistance <= (double)DistanceRange::COMING_IN_RANGE) {
			if (counter == 1) {
				SetWayPoint(target->ReferCoord2D_d(), target->ReferRadianOnZ(),
					target->ReferSpeedOnZ());
			}
		}
		CalTargetRadian(); 
	}
	else {
		DisableWayPoint_MoveWithEnemy(target->ReferRadianOnZ());
	}
	SetRadianNeeded();//�p�x���X�V
	me->SetEngineOutPutRate(SpeedINeed(target->ReferOutPutRate()));
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
	if (speed == 0)
		speed = 0.1;
	NextPoint(&wayPoint,radian, speed, nextPointFrame);
}

/*�G�Ƌ߂Â��ƓG�̐i�s�����Ɠ��������ɐi��*/
void FlagShipAI::DisableWayPoint_MoveWithEnemy(double radian) {
	targetRadian = radian - nowRadian;
	targetRadian = fmod(targetRadian, MathAndPhysics::PI);
}

/*�ڕW�Ƃ̊p�x���v�Z����*/
void FlagShipAI::CalTargetRadian() {
	targetRadian = CalRadianBetweenPoints(wayPoint, myPos, nowRadian);
}

void FlagShipAI::SetRadianNeeded() {
	using namespace MathAndPhysics;
	/*�͈͂��獡�K�v�Ȋp�x���v�Z����*/
	if (abs(targetRadian) > RadianRange::RANGE_MAX * OneDegreeRadian) {
		radianNeededNow = RadianRange::SPEED_MAX * OneDegreeRadian;
	}
	if (abs(targetRadian) <= RadianRange::RANGE_MAX * OneDegreeRadian&&
		abs(targetRadian) > RadianRange::RANGE_1_2 * OneDegreeRadian) {
		radianNeededNow = RadianRange::SPEED_1_2 * OneDegreeRadian;
	}
	if (abs(targetRadian) <= RadianRange::RANGE_1_2 * OneDegreeRadian&&
		abs(targetRadian) > RadianRange::RANGE_1_4 * OneDegreeRadian) {
		radianNeededNow = RadianRange::SPEED_1_4 * OneDegreeRadian;
	}
	if (abs(targetRadian) <= RadianRange::RANGE_1_4 * OneDegreeRadian&&
		abs(targetRadian) > RadianRange::RANGE_1_8) {
		radianNeededNow = RadianRange::SPEED_1_8 * OneDegreeRadian;
	}
	if (targetRadian == RadianRange::RANGE_1_8) {
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

double FlagShipAI::SpeedINeed(double outPutRate) {
	/*�p�g���[�����̏o�͔͂���*/
	if (targetDistance >= (double)DistanceRange::PATROL_RANGE) {
		return outPut_50;
	}
	/*�ڋ߂��鎞��0.75*/
	if (targetDistance < (double)DistanceRange::COMING_IN_RANGE&&
		targetDistance >= (double)DistanceRange::TAKE_T) {
		return outPut_75;
	}
	/*�O�̈ʒu����鎞�͑S��*/
	if (targetDistance < (double)DistanceRange::TAKE_T&&
		targetDistance >= (double)DistanceRange::BATTLE) {
		return outPut_100;
	}
	/*�퓬�̎��͓G�̑��x�ɍ��킹��*/
	if (targetDistance < (double)DistanceRange::BATTLE) {
		return outPutRate;
	}
}