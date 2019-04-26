#pragma once
#include"DefinedData.h"
#include"AI_Action.h"
#include"ShipMain.h"
#include"IncludeForAll.h"
#include"OtherFunctions.h"
class FlagShipAI
{
public:
	FlagShipAI() :inBattle(false) {}
	~FlagShipAI();

	void LetUsGo(ShipMain *ship);//�ړ��֐�-�X�e�[�^�X�ɂ���Č��߂��s�����s��
	void ShowMeTheTarget();//�G���w�肷��֐�

	double ReferRadianNeededNow() { return radianNeededNow; }

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
	/*�֐�����*/
	void CalData(ShipMain *ship);
	void SetWayPoint();//�E�F�C�|�C���g�ݒ�֐�
	void SetWayPoint(Coordinate2D<double>, double radian, double speed);
	void SetNowRadian(double rad) { nowRadian = rad; }
	void SetMyPos(Coordinate2D<double> pos) { myPos = pos; }
	void CalTargetRadianAndSetRadianNeeded();
	/*�ϐ�����*/
	Coordinate2D<double> wayPoint;//�E�F�C�|�C���g
	Coordinate2D<double> myPos;//�������̍��W
	double targetRadian;//�E�F�C�|�C���g�Ɍ����p�x
	double nowRadian;//�������̊p�x
	double radianNeededNow;//��]�p�x//targetRadian�ɋ߂Â��΋߂Â��قǏ������Ȃ�
	double nowSpeed;//�������̑��x
	double distance;//�E�F�C�|�C���g�Ƃ̋���

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

