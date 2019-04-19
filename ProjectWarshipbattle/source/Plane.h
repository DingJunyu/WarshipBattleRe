#pragma once
#include"AllMovableObjects.h"
#include"AirWeapon.h"
#include"DefinedData.h"
/*���ׂĂ̔�s�@�̊��N���X*/
class Plane :
	public AllMovableObjects
{
public:
	Plane() : AllMovableObjects(true, true, false, false),
		takedOff(false) {}
	~Plane();

	//�₢���킹�֐�
	int ReferMaxFuel() { return maxFuel; }
	int ReferFuelRemained() { return fuelRemained; }

	//�ė���Ԃ֕ω�
	void FallingDown() { UnsetPlane(); Killed(); }

	void SetMSWeightPower(double Ms, double W, double P);
	void CalSpeed();
	void TakeOff(double X,double Z, double R);

private:
	int maxFuel;
	int fuelRemained;

	double maxSpeed;
	double weight;
	double power;
	double acceleratedSpeed;

	bool takedOff;//��ԏ��
	int status;

	//�ė�����p�摜�n���h��
	int *FallingDownHandle;
};

