#include "ShipCrashParts.h"

ShipCrashParts::~ShipCrashParts()
{
}

void ShipCrashParts::MinusHP(int damage) {
	//パーツのHPが減る時は生きる状態を確認
	heartPoint -= damage;
	if (heartPoint <= 0)
		damaged = true;
}