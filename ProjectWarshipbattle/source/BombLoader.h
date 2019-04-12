#pragma once
#include "AirWeapon.h"
#include"DefinedData.h"
#include"Bomb.h"
class BombLoader :
	public AirWeapon
{
public:
	BombLoader() :AirWeapon(BOMB_PW){}
	~BombLoader();

	//”š’e‚ğ“Š‰º‚·‚éŠÖ”
	Bomb DropTheBomb();

	//•Ší‚ğ‰Šú‰»‚·‚é
	void Inif(Bomb B, int Amount) { bombOnThisPlane = B;
		SetAmmo(Amount); }

private:
	Bomb bombOnThisPlane;
};

