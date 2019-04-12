#pragma once
#include"AllMovableObjects.h"
#include"AirWeapon.h"
#include"DefinedData.h"
class Plane :
	public AllMovableObjects
{
public:
	Plane() : AllMovableObjects(true, true, false, false),
		takedOff(false) {}
	~Plane();

	//–â‚¢‡‚í‚¹ŠÖ”
	int ReferMaxFuel() { return maxFuel; }
	int ReferFuelRemained() { return fuelRemained; }

	//’Ä—ó‘Ô‚Ö•Ï‰»
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

	bool takedOff;//”ò‚Ôó‘Ô
	int status;

	//’Ä—“®‰æ—p‰æ‘œƒnƒ“ƒhƒ‹
	int *FallingDownHandle;
};

