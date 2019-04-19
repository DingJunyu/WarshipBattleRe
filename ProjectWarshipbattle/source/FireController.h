#pragma once
#include"DefinedData.h"
#include"Ammo.h"
#include"AllMovableObjects.h"
#include"OtherFunctions.h"
/*���̃N���X�͊��I�u�W�F�N�g�̈ړ������𗘗p���āA�C�e�Ȃǂ̗����_��\�����܂�*/

class FireController
{
public:
	FireController();
	~FireController();

	/*�ݒu�Ȃ�*/
	void SetSpeed(double Speed) { 
		speed = Speed;
	}//���x��ݒu
	void SetRadian(RadianNeededIn3D temp) {
		radian = temp;
		testAmmo.SetRadianOnY(temp.y);
		testAmmo.SetRadianOnZ(temp.z);
	}//�p�x��ݒu
	void SetCoord(Coordinate<double> Coord) {
		testAmmo.SetCoord(Coord);
	}//���W��ݒu

	/*�₢���킹�֐�*/
	Coordinate<double> ReferAnswer() { return answer; }
	double ReferMaxRange() { return distance[FIRE_DISTANCE::ANGLE_100_P]; }
	double ReferMinRange() { return distance[FIRE_DISTANCE::ANGLE_0_P]; }

	/*�v�Z�֐�*/
	void InifDistance(double MaxRadian,double MinRadian);
	void CalTheAnswer();

	double CalDistanceAndTellMeRadianOnY(double Distance);

private:
	double FindTheDistance(double Distance, double MaxRadian, double MinRadian);

	double speed;
	
	RadianNeededIn3D radian;//�p�x
	AllMovableObjects testAmmo;//�V�~�����[�V�����p�I�u�W�F�N�g

	double distance[FIRE_DISTANCE::ANGLE_TYPE_AMOUNT];
	
	double maxRadian;//�����ő�p�x
	double minRadian;//�����ŏ��p�x
	double radianRange;

	Coordinate<double> originPoint;
	Coordinate<double> answer;//�����n�_�̍��W
};
