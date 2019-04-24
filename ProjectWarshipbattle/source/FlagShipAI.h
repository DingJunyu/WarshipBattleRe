#pragma once
#include"DefinedData.h"
#include"AI_Action.h"
#include"IncludeForAll.h"
class FlagShipAI
{
public:
	FlagShipAI() :inBattle(false) {}
	~FlagShipAI();

	void LetUsGo();//�ړ��֐�-�X�e�[�^�X�ɂ���Č��߂��s�����s��
	void LetUsFight();//�퓬�֐�

private:
	/*��ԕ���*/
	bool inBattle;

	/*�ړ�����*/
	const double nextPosOnMapX = 1500;
	const double nextPosOnMapZ = 1500;
	const int randRange = 1000;

	void SetMyPos(Coordinate2D<double>);
	void SetWayPoint();//�E�F�C�|�C���g�ݒ�֐�
	void SetWayPoint(Coordinate2D<double>);
	Coordinate2D<double> wayPoint;
	Coordinate2D<double> myPos;
	double targetRadian;//�E�F�C�|�C���g�Ɍ����p�x
	double nowRadian;
	double radianNeededNow;//��]�p�x//targetRadian�ɋ߂Â��΋߂Â��قǏ������Ȃ�

	/*�͑��̐ڐG��Ԃɂ��*/
	double targetSpeed;
	double minMaxSpeedInFleet;//�͑��̒��Ɉ�Ԓx���D�̍ő呬�x

	/*�v�Z����*/
	void CalRadianNeeded();//�E�F�C�|�C���g�֍s�����ߕK�v�Ȋp�x�̍�
	void CalDistanceToOpponent();//����Ƃ̋������v�Z����

	/*�퓬����*/
	void ChooseTarget();//�G��I��

};

