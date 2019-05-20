#include "ArtificialIntelligence.h"
using namespace std;
/*AI*/
ArtificialIntelligence::ArtificialIntelligence()
{
	flagShip = false;
}

ArtificialIntelligence::~ArtificialIntelligence()
{
}

void ArtificialIntelligence::Move(ShipMain me, ShipMain target) {
	SetMyPos(me.ReferCoord2D_d());//�ʒu��ݒ肷��
	SetNowRadian(me.ReferRadianOnZ());
	SetTargetPos(target.ReferCoord2D_d(), target.ReferRadianOnZ());//�E�F�C�|�C���g��ݒu����
	CalWaypointDis();//������}��
	CalTargetRadian();//�p�x���v�Z����
	SetRadianNeeded();//�K�v�Ȋp�x���v�Z����
	SetSpeed(target.ReferOutPutRate());//�K�v�ȑ��x��ݒu����
}

void ArtificialIntelligence::InBattle(ShipMain *me,
	vector<ShipMain> shipList,int targetNum) {
	double maxRange = me->ReferMainMaxRange();
	targetDis = MathAndPhysics::Infinite;//������ɐݒu����
	double temp;
	int count = 0;//�������Z�b�g����
	if (targetNum != -1) {
		me->fireDataFigureUp.SetNumber(targetNum);//�^�[�Q�b�g��ݒu����
	}
	else {
		for (auto ship = shipList.begin();
			ship != shipList.end();
			ship++) {
			if (ship->ReferAlive()) {
				temp = Distance2D(me->ReferCoord2D_d(), ship->ReferCoord2D_d());//�����𑪂�
				if (temp < targetDis) {//���̋�����菬�������
					targetDis = temp;//��ԋ߂��G��I��
					me->fireDataFigureUp.SetNumber(count);
				}			}
			count++;//���Ԃ𑝂₷
		}
	}
}

void ArtificialIntelligence::SetTargetPos(Coordinate2D<double> target,double radian) {
	wayPoint = target;//�E�F�C�|�C���g��������
	NextPoint(&wayPoint, radian, disToFront);//�p�x�Ƌ����𗘗p���ăE�F�C�|�C���g���X�V���܂�
}

void ArtificialIntelligence::CalTargetRadian() {
	targetRadian = CalRadianBetweenPoints(wayPoint, myPos, nowRadian);
}

void ArtificialIntelligence::SetRadianNeeded() {
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
	if (targetRadian > 0)
		radianNeededNow = -radianNeededNow;
}

void ArtificialIntelligence::CalDistance(Coordinate2D<double> coord) {
	targetDis = Distance2D(coord, myPos);//�ڕW�����𑪂�
}

void ArtificialIntelligence::CalWaypointDis() {
	wayPointDis = Distance2D(myPos, wayPoint);//�E�F�C�|�C���g�Ƃ̋����𑪂�
}

void ArtificialIntelligence::SetSpeed(double TGoutputRate) {
	/*�E�F�C�|�C���g�Ƃ̋����ɂ���đ��x���ς��܂�*/
	if (wayPointDis >= DistanceRange::CLOSING)
		outPutRate = 1;
	else if (wayPointDis <= DistanceRange::SLOW_DOWN
		&& wayPointDis > DistanceRange::STOP)
		outPutRate = 0.25;
	else if (wayPointDis <= DistanceRange::STOP)
		outPutRate = -0.3;
	else
		outPutRate = TGoutputRate - 0.05;
}