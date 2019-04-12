#pragma once
#include"Ammo.h"
#include"Torpedo.h"
class Weapon
{
public:
	Weapon();
	Weapon(double RelativeCoordX, double RelativeCoordZ, double Length, double Width,
		double RadianOnZ, double RadianOnY,int MaxHeartPoint,double ArmorOnTheSide,
		double ArmorOnTheTop,int *AmmoHandle,int InitialSpeed,double High,int SerialNumber) :
		relativeCoordX(RelativeCoordX),relativeCoordZ(RelativeCoordZ),length(Length),
		width(Width),radianOnZ(RadianOnZ),radianOnY(RadianOnY),heartPoint(MaxHeartPoint),
		maxHeartPoint(MaxHeartPoint),armorOntheSide(ArmorOnTheSide),
		armorOntheTop(ArmorOnTheTop),
		ammoHandle(AmmoHandle), initialSpeed(InitialSpeed), high(High), 
		serialNumber(SerialNumber) {
		lastShootedTime = 0;
		maxRadianOnY = MathAndPhysics::PI * 0.40;
	}
	~Weapon();

	//�f�[�^�₢���킹�֐�
	int ReferNumbefofGuns() { return numberofGuns; }
	int ReferReloadTime() { return reloadTime; }
	int ReferLastShootedTime() { return lastShootedTime; }
	double ReferCalibre() { return calibre; }
	double ReferRelativeCoordX() { return relativeCoordX; }
	double ReferRelativeCoordZ() { return relativeCoordZ; }
	double ReferLength() { return length; }
	double ReferWidth() { return width; }
	double ReferRadianOnZ() { return radianOnZ; }
	double ReferRadianOnY() { return radianOnY; }
	double ReferMaxRadianOnY() { return maxRadianOnY; }
	double ReferHP() { return heartPoint; }
	double ReferMaxHP() { return maxHeartPoint; }
	double ReferArmorOntheSide() { return armorOntheSide; }
	double ReferArmorOntheTop() { return armorOntheTop; }

	void SetCoolDownTime(int time) { reloadTime = time; }
	void Turn(bool right) {
		if (right)
			radianOnZ += MathAndPhysics::PI * (1.0 / 180.0);
		else
			radianOnZ -= MathAndPhysics::PI * (1.0 / 180.0);
	}
	bool Pull(bool up);

	//��Ԗ₢���킹�֐�
	bool ReferShootable();//�ˌ��ł����true��߂�
	
	Ammo Shoot(double x, double y,double r);

private:
	int numberofGuns;//�C���̘A����
	unsigned int reloadTime;//�đ��U�̎���
	unsigned int lastShootedTime;//�O��ˌ���������
	double calibre;
	int serialNumber;

	int *turrentHandle;//�摜�f�[�^�ۑ��p
	int *ammoHandle;

	double initialSpeed;//�����x
	
	double relativeCoordX;
	double relativeCoordZ;
	double length;
	double width;
	double radianOnZ;//�����ʂ̊p�x
	double radianOnY;//�C�g�̊p�x-->����ŖC�e�����ʒu�𐄒肷��
	double maxRadianOnY;

	double high;

	int heartPoint;
	int maxHeartPoint;
	double armorOntheSide;
	double armorOntheTop;
};