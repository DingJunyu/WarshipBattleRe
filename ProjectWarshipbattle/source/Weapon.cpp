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

	//��΍��W���v�Z����
	double realX = cos(radianOnZ + r) * relativeCoordX -
		sin(radianOnZ + r) * relativeCoordZ + x;
	double realZ = cos(radianOnZ + r) * relativeCoordZ +
		sin(radianOnZ + r) * relativeCoordX + z;

	//�ŏ��̊p�x�������_���ɕύX���čŌ�̗����_�ɉe�����グ��悤�ɂ���
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
	if (lastShootedTime + reloadTime < GetTickCount()) {//�O��ˌ��̎��Ԃƍ��̎��Ԃ��ƍ�
		return true;
	}
	return false;
}

/*����X�e�[�^�X����*/
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