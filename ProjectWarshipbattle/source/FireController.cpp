#include "FireController.h"

FireController::FireController() :testAmmo(true, false, true, true)
{
}

FireController::~FireController()
{
}

/*���������鎞�K�����̊֐����Ăяo��*/
void FireController::InifDistance(double MaxRadian, double MinRadian) {
	/*�p�x�͈̔͂ł�*/
	/*�ő�p�x�ƍŏ��p�x�͂��ׂĐ����Ɨ\�z�ł��邪�A
	�����ɂȂ�\�����[���̂킯�ł͂Ȃ��̂ŁA
	�f�o�b�O�̎��ɂ����ӂ��Ȃ��Ƃ����Ȃ�*/
	radianRange = MaxRadian - MinRadian;
	/*�͈͂�ۑ�����*/
	maxRadian = MaxRadian;
	minRadian = MinRadian;
	originPoint = { 0,20,0 };//���_

	for (int i = FIRE_DISTANCE::ANGLE_100_P; i <= FIRE_DISTANCE::ANGLE_0_P; i++) {
		testAmmo.SetSpeed(speed);//���x��ݒu����
		testAmmo.SetRadianOnY(radianRange*(1 - i * 0.2) + MinRadian);//�p�x��ݒu����
		testAmmo.SetCoord(originPoint);//���W��0�ɂ���
		CalTheAnswer();//�����n�_��^���Čv�Z����
		distance[i] = Distance2D(originPoint,answer);//�������v�Z����
	}
}

void FireController::CalTheAnswer() {
	testAmmo.SetSpeed(speed);
	while (testAmmo.ReferCoordY() > 0) {
		testAmmo.Move();//�C�ʂɗ�����܂ňړ�����
	}
	answer = testAmmo.ReferCoord();//�������W���L�^����
}

/*���̊֐��ɂ��āF
�܂����ׂĂ̏󋵂̌��؂͊������Ȃ��������߁A��肪�N����\�����\�z�ł���*/
double FireController::CalDistanceAndTellMeRadianOnY(double Distance) {
	for (int i = FIRE_DISTANCE::ANGLE_100_P; i < FIRE_DISTANCE::ANGLE_0_P; i++) {
		/*�������̋����͂��͈͓̔��ł����*/
		if (Distance >= distance[i + 1] && Distance <= distance[i]) {
			return FindTheDistance(Distance, radianRange*(1 - i * 0.2) + minRadian,
				radianRange*(1 - (i + 1)*0.2) + minRadian);//�v�Z�����p�x��Ԃ�
		}
	}
	/*�͈͒�������A�ő�������͍ŏ�������Ԃ�*/
	if (Distance > distance[FIRE_DISTANCE::ANGLE_100_P])
		return maxRadian;
	if (Distance < distance[FIRE_DISTANCE::ANGLE_0_P])
		return minRadian;
}

/*�񕪖@�ŋ�̊p�x���v�Z����*/
double FireController::FindTheDistance(double Distance, 
	double MaxRadian, double MinRadian) {

	double realRadian = (MaxRadian - MinRadian) / 2 + MinRadian;//�񕪖@�Ōv�Z����
	
	/*�e�X�g�f�[�^��ݒu����*/
	testAmmo.SetSpeed(speed);
	testAmmo.SetRadianOnY(realRadian);
	testAmmo.SetCoord(originPoint);

	CalTheAnswer();//���͋[���s��
	
	double tempDistance;
	tempDistance = Distance2D(originPoint, answer);

	double result = tempDistance - Distance;//�����v�Z����

	/*�\�z�ł�*/
	if (abs(result) < MathAndPhysics::RANGE_ERROR)//�덷�͈͓��ɓ�������Ԃ�
		return realRadian;

	if (result > 0) {
		return FindTheDistance(Distance, realRadian, MinRadian);
		//��菬�����̕����ɒT��
	}
	else{
		return FindTheDistance(Distance, MaxRadian, realRadian);
		//���傫���̕����ɒT��
	}
}