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

	//���e�𓊉�����֐�
	Bomb DropTheBomb();

	//���������������
	void Inif(Bomb B, int Amount) { bombOnThisPlane = B;
		SetAmmo(Amount); }

private:
	Bomb bombOnThisPlane;
};

