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

	//データ問い合わせ関数
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

	//状態問い合わせ関数
	bool ReferShootable();//射撃できればtrueを戻す
	
	Ammo Shoot(double x, double y,double r);

private:
	int numberofGuns;//砲塔の連装数
	unsigned int reloadTime;//再装填の時間
	unsigned int lastShootedTime;//前回射撃した時間
	double calibre;
	int serialNumber;

	int *turrentHandle;//画像データ保存用
	int *ammoHandle;

	double initialSpeed;//初速度
	
	double relativeCoordX;
	double relativeCoordZ;
	double length;
	double width;
	double radianOnZ;//水平面の角度
	double radianOnY;//砲身の角度-->これで砲弾落下位置を推定する
	double maxRadianOnY;

	double high;

	int heartPoint;
	int maxHeartPoint;
	double armorOntheSide;
	double armorOntheTop;
};