#include "Weapon.h"

Weapon::Weapon()
{
}

Weapon::~Weapon()
{
}

Coordinate<double> Weapon::ReferRealCoord(Coordinate<double> Coord, double r) {
	Coordinate<double> temp;
	temp.x = cos(radianOnZ + r) * relativeCoordX -
		sin(radianOnZ + r) * relativeCoordZ + Coord.x;
	temp.z= cos(radianOnZ + r) * relativeCoordZ +
		sin(radianOnZ + r) * relativeCoordX + Coord.z;
	temp.y = high;
	return temp;
}

Ammo Weapon::Shoot(double x, double z,double r) {
	lastShootedTime = GetTickCount();

	//絶対座標を計算する
	double realX = cos(radianOnZ + r) * relativeCoordX -
		sin(radianOnZ + r) * relativeCoordZ + x;
	double realZ = cos(radianOnZ + r) * relativeCoordZ +
		sin(radianOnZ + r) * relativeCoordX + z;

	//最初の角度がランダムに変更して最後の落下点に影響を上げるようにする
	double realRadianOnZ = radianOnZ + r;
	double targetRandom = rand() % 20;
	if (targetRandom >= 10) {
		realRadianOnZ += RandomDataWhenShoot::radianOnZ * (targetRandom - 9) / 10;
	}
	else {
		realRadianOnZ -= RandomDataWhenShoot::radianOnZ * targetRandom / 10;
	}

	return Ammo(initialSpeed, realRadianOnZ, realX, realZ, high, radianOnY,
		ammoHandle, serialNumber,damage);
}

bool Weapon::ReferShootable() {
	if (lastShootedTime + reloadTime < GetTickCount()) {//前回射撃の時間と今の時間を照合
		return true;
	}
	return false;
}

/*武器ステータス調整*/
bool Weapon::Pull(bool up) {
	if (up) {
		if (radianOnY <= maxRadianOnY) {
			radianOnY += MathAndPhysics::PI*(double)(0.25 / 180.0);
			return false;
		}
		radianOnY = maxRadianOnY;
		return true;
	}
	else {
		if (radianOnY >= 0) {
			radianOnY -= MathAndPhysics::PI*(double)(0.25 / 180.0);
			return false;
		}
		radianOnY = 0;
		return true;
	}
}