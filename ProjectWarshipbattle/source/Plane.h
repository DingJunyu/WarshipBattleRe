#pragma once
#include"AllMovableObjects.h"
#include"AirWeapon.h"
#include"DefinedData.h"
/*すべての飛行機の基底クラス*/
class Plane :
	public AllMovableObjects
{
public:
	Plane() : AllMovableObjects(true, true, false, false),
		takedOff(false) {}
	~Plane();

	//問い合わせ関数
	int ReferMaxFuel() { return maxFuel; }
	int ReferFuelRemained() { return fuelRemained; }

	//墜落状態へ変化
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

	bool takedOff;//飛ぶ状態
	int status;

	//墜落動画用画像ハンドル
	int *FallingDownHandle;
};

