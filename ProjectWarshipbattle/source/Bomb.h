#pragma once
#include "AllMovableObjects.h"
/*爆弾クラス*/
class Bomb :
	public AllMovableObjects
{
public:
	Bomb() : AllMovableObjects(true, false, true, true) {}
	~Bomb();

private:
	double damageRange;//爆発範囲
	double damage;//ダメージ
};

