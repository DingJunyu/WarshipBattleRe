#pragma once
#include"AllMovableObjects.h"
#include"ShipCrashParts.h"
#include"DefinedData.h"
#include"IncludeForAll.h"
#include"Weapon.h"
#include"Camera.h"
#include"EffectPoint.h"
#include"ShipData.h"
#include"Engine.h"
#include"EffectTemplate.h"
#include"SoundLoader.h"
class ShipMain :
	public AllMovableObjects
{
public:
	ShipMain() : AllMovableObjects(false, false, false, true) {
		currentAccPercentage = 0;
		returnToCenter = false;

		/*テスト部分*/
		hitPoint = 100000;
		shipMainCrash = { 245, 30, 24 };
	}
	~ShipMain();

	//初期化
	void RegistrateShipCrashParts();
	void InifThisShip(int *ShipHandle,int *SShadowH,int ShipNum, 
		EffectTemplate ET,SoundLoader *SL);
	void SetEffectPoint(ShipData SD);
	void DestroyMemory();
	void SetWeaponTest(PictureLoader *PL);
	void SetWeapon();

	//データ操作関数
	void SetSerialNumber(int x) { serialNumber = x; }

	//コントロール
	void ControlThisShip(int Command);

	//移動関連
	void ChangeAccPercentage(bool up);
	void CalSpeed();
	void ChangeDirect(bool right);
	void ReturnDirectChange() { returnToCenter = ! returnToCenter; }
	void Alignment();

	void TEST();
	void TestDraw(double x,double z);

	/*簡単化した戦闘*/
	void SufferDamage(int damage);

	//問い合わせ
	/*移動関連*/
	bool ReferReturnOn() { return returnToCenter; }
	double ReferChangingRadian() { return currentRadian; }
	double ReferOutPutRate() { return currentAccPercentage; }
	/*武器関連*/
	int ReferWeaponCount(bool Main) { 
		if (Main)
			return MainWeaponCount;
		else
			return SubWeaponCount;
	}
	double ReferMainWeaponRadianOnZ() {
		return MainWeapon[0].ReferRadianOnZ();
	}
	double ReferMainWeaponCD() {
		return
			GetTickCount() - MainWeapon[0].ReferLastShootedTime();
	}
	double ReferMainWeaponCoolDownTime() {
		return MainWeapon[0].ReferReloadTime();
	}
	double ReferMainWeaponRadianOnY() {
		return MainWeapon[0].ReferRadianOnY();
	}
	double ReferMainMaxWeaponRadianOnY() {
		return MainWeapon[0].ReferMaxRadianOnY();
	}
	/*collision関連*/
	double ReferShipCrashR() { return shipCrashR; }
	CrashSize3d<double> ReferShipCrashSize() { return shipMainCrash; }
	int ReferCollisionPointAmount() { return pointCount; }
	Coordinate2D<double> ReferCollisionPoint(int num) {
		return aroundPointPos[num];
	}
	/*番号*/
	int ReferSerialNumber() { return serialNumber; }
	//エンジン関連
	double ReferEngineOutput() { return mainEngine.ReferOutput(); }
	double ReferEngineMaxOutput() { return mainEngine.ReferMaxOutput(); }

	//エフェクト生成
	Effect NewBubble(int num);
	Effect NewSmoke(int num);
	int ReferBubblePointCount() { return bubblePointCount; }

	void CheckAndPlaySound();

	/*射撃*/
	int HowManyWeaponYouHave() { return MainWeaponCount; }
	bool IsThisOneUsable(int Num, bool Main);
	Ammo Shoot(int Num, bool Main);



private:
	ShipCrashParts * MainParts;
	ShipCrashParts * SubParts;
	double shipCrashR;
	CrashSize3d<double> shipMainCrash;
	/*自分の弾が自分に当たらないように使用する番号です*/
	int serialNumber;

	Weapon * MainWeapon;//メイン武器
	int MainWeaponCount;//メイン武器の数
	Weapon * SubWeapon;//サブ武器
	int SubWeaponCount;//サブ武器の数

	EffectPoint * bubbleStartPoint;//水泡生成ポイント
	int bubblePointCount;//水泡生成ポイントの数

	/*あたり判定・泡生成用点*/
	Coordinate2D<double> aroundPointPos[AUTO_SHIP_DATA::AROUND_POINT];
	int pointCount;
	void FindThosePoint();

	EffectPoint * smokeStartPoint;
	int smokePointCount;

	void MemorySecure();

	/*エンジン*/
	Engine mainEngine;
	double currentAccPercentage;//出力のパーセンテージ

	/*砲のステータス変更*/
	bool TurnMainWeapon(bool right);
	bool PullMainWeapon(bool up);

	double draft;//喫水:魚雷を使う時に使うデータです。
	int shipType;

	int *shipHandle[SerialNumberOfShipHandle::ARRAY_SIZE];

	//方向舵
	double maxRadian;
	double currentRadian;
	double radianChangePerFrame;
	bool returnToCenter;
	
	std::string name;

	/*音声関連*/
	/*ホードの種類はまだ決まってないので今は使っていない*/
	/*soundloaderの構造を更新した後また更新する*/
	void LoadSound(SoundLoader *SL);
	int *soundEngine;
	int *soundMoving;
	int *soundSlow;
	int *soundTurning;
	int HordType;
	int *soundHordHigh;
	int *soundHordLow;

	/*簡単化した戦闘部分*/
	int hitPoint;
};