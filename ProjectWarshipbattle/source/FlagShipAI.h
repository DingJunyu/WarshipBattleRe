#pragma once
#include"DefinedData.h"
#include"AI_Action.h"
#include"ShipMain.h"
#include"IncludeForAll.h"
#include"OtherFunctions.h"
#include<cmath>

class FlagShipAI
{
public:
	FlagShipAI() :inBattle(false) {}
	~FlagShipAI();

	void LetUsGo(ShipMain *me, ShipMain *target);//�ړ��֐�-�X�e�[�^�X�ɂ���Č��߂��s�����s��
	void ShowMeTheTarget();//�G���w�肷��֐�

	double ReferRadianNeededNow() { return radianNeededNow; }
	double ReferSpeedInNeed() { return targetSpeedRate; }

private:
	/*��ԕ���*/
	bool inBattle;
	bool concentrateOn;/*��̃^�[�Q�b�g�ɏW�����邩*/

	/******************�ړ�����*****************/
	/*�萔����*/
	const double nextPosOnMapX = 1500;
	const double nextPosOnMapZ = 1500;
	const int randRange = 1000;
	const int nextPointFrame = 1800;//18�b��̈ʒu���^�[�Q�b�g�ɐݒ肷��
	const double outPut_50 = 0.5;
	const double outPut_75 = 0.75;
	const double outPut_100 = 1.0;
	const int nextCounter = 600;
	/*�֐�����*/
	void CalData(ShipMain *ship);
	void SetWayPoint();//�E�F�C�|�C���g�ݒ�֐�
	void SetWayPoint(Coordinate2D<double>, double radian, double speed);
	void DisableWayPoint_MoveWithEnemy(double radian);
	void SetNowRadian(double rad) { nowRadian = rad; }
	void SetMyPos(Coordinate2D<double> pos) { myPos = pos; }
	void CalTargetRadian();//�G�Ƃ̊p�x���v�Z����
	void SetRadianNeeded();// �����K�v�Ȋp�x��ݒ肷��
	void CalDistance();//
	double SpeedINeed(double outPutRate);//�o�͂�ݒ肷��
	/*�ϐ�����*/
	Coordinate2D<double> wayPoint;//�E�F�C�|�C���g
	Coordinate2D<double> myPos;//�������̍��W
	double targetRadian;//�E�F�C�|�C���g�Ɍ����p�x
	double nowRadian;//�������̊p�x
	double radianNeededNow;//��]�p�x//targetRadian�ɋ߂Â��΋߂Â��قǏ������Ȃ�
	double nowSpeed;//�������̑��x
	double targetSpeedRate;
	double distance;//�E�F�C�|�C���g�Ƃ̋���
	double targetDistance;//�G�Ƃ̋���
	const double needToChange = 300;
	int counter;

	/*�͑��̐ڐG��Ԃɂ��*/
	double targetSpeed;
	double minMaxSpeedInFleet;//�͑��̒��Ɉ�Ԓx���D�̍ő呬�x

	/*�v�Z����*/
	void CalRadianNeeded();//�E�F�C�|�C���g�֍s�����ߕK�v�Ȋp�x�̍�
	void CalDistanceToOpponent();//����Ƃ̋������v�Z����

	/*�퓬����*/
	void ChooseTarget();//�G��I��
	const double detectDistance = 2000;

};

