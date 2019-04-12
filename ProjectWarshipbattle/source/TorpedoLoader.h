#pragma once
#include "AirWeapon.h"
#include"DefinedData.h"
#include"Torpedo.h"
class TorpedoLoader :
	public AirWeapon
{
public:
	TorpedoLoader() :AirWeapon(TORPEDO_PW) {}
	~TorpedoLoader();

	//‹›—‹‚ğ“Š‰º‚·‚éŠÖ”
	Torpedo DropTheTorpedo();

	//•Ší‚ğ‰Šú‰»‚·‚é
	void Inif(Torpedo T, int Amount) { torpedoOnThisPlane = T; 
		SetAmmo(Amount); }

private:
	Torpedo torpedoOnThisPlane;
};

