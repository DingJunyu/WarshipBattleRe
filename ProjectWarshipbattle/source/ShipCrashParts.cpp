#include "ShipCrashParts.h"

ShipCrashParts::~ShipCrashParts()
{
}

void ShipCrashParts::MinusHP(int damage) {
	//�p�[�c��HP�����鎞�͐������Ԃ��m�F
	heartPoint -= damage;
	if (heartPoint <= 0)
		damaged = true;
}