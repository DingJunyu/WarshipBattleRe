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

	void SetSpeed(double Speed) { testAmmo.SetSpeed(Speed); }//���x��ݒu
	void SetRadian(RadianNeededIn3D temp) {
		radian = temp;
		testAmmo.SetRadianOnY(temp.y);
		testAmmo.SetRadianOnZ(temp.z);
	}//�p�x��ݒu
	void SetCoord(Coordinate<double> Coord) {
		testAmmo.SetCoord(Coord);
	}//���W��ݒu

	Coordinate<double> ReferAnswer() { return answer; }
	void CalTheAnswer();

private:
	RadianNeededIn3D radian;//�p�x
	AllMovableObjects testAmmo;//�V�~�����[�V�����p�I�u�W�F�N�g

	Coordinate<double> answer;//�����n�_�̍��W
};

