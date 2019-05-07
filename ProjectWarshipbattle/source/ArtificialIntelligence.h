#pragma once
#include"DefinedData.h"
#include"ShipMain.h"
#include"OtherFunctions.h"
#include"AI_Action.h"
#include"Ammo.h"
#include<vector>
#include<list>
/*���̃N���X�ɂ͂`�h�̍s������������*/
class ArtificialIntelligence
{
public:
	ArtificialIntelligence();
	~ArtificialIntelligence();

	void SetStatus(ShipMain ship);
	void Move(ShipMain me,ShipMain target);//�t���O�V�[�v�ȊO�̓t���O�V�[�v��ڕW�ɂ��Ĉړ�����
	void InBattle(ShipMain *me,std::vector<ShipMain> shipList,
		int targetNum);

	void SetFlagShip() { flagShip = true; }//�t���O�V�[�v�t���O��ݒu
	bool ReferFlagShip() { return flagShip; }

	//�D��Ԗ₢���킹�֐�
	double ReferRadianNeededNow() { return radianNeededNow; }
	double ReferOutPutRateNeededNow() { return outPutRate; }

private:
	const double disToFront = 50;

	double range;//
	double targetDis;//�ڕW�Ƃ̋���
	double wayPointDis;//�E�F�C�|�C���g�Ƃ̋���
	Coordinate2D<double> wayPoint;//�E�F�C�|�C���g
	Coordinate2D<double> myPos;//�����̈ʒu
	bool flagShip;//�t���O�V�[�v�t���O
	double nowRadian;//���̊p�x
	double targetRadian;//�ڕW�p�x
	double radianNeededNow;//�]��p�x
	double outPutRate;//���x

	void SetTargetPos(Coordinate2D<double> target,double radian);//�E�F�C�|�C���g�ݒu�֐�
	void SetMyPos(Coordinate2D<double> pos) { myPos = pos; }//�����̈ʒu�ݒu�֐�
	void SetNowRadian(double rad) { nowRadian = rad; }
	void CalTargetRadian();
	void SetRadianNeeded();
	void CalDistance(Coordinate2D<double> coord);
	void CalWaypointDis();
	void SetSpeed(double outputRate);
};

