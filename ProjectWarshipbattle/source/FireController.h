#pragma once
#include"DefinedData.h"
#include"Ammo.h"
#include"AllMovableObjects.h"
/*���̃N���X�͊��I�u�W�F�N�g�̈ړ������𗘗p���āA�C�e�Ȃǂ̗����_��\�����܂�*/

class FireController
{
public:
	FireController();
	~FireController();

	void SetSpeed(double Speed) { speed = Speed; }//���x��ݒu
	void SetRadian(double RadianOnZ, double RadianOnY) {
		radian.y = RadianOnY;
		radian.z = RadianOnZ;
	}//�p�x��ݒu
	void SetCoord(Coordinate<double> Coord) {
		testAmmo.SetCoord(Coord);
	}//���W��ݒu

	Coordinate<double> ReferAnswer() { return answer; }


private:
	void CalTheAnswer();

	double speed;//���x
	RadianNeededIn3D radian;//�p�x
	AllMovableObjects testAmmo;//�V�~�����[�V�����p�I�u�W�F�N�g

	Coordinate<double> answer;//�����n�_�̍��W
};

