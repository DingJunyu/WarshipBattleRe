#pragma once
#include"AllMovableObjects.h"//基底クラス
#include"ShipCrashParts.h"//ＨＰ計算用パーツ
#include"DefinedData.h"//定数
#include"IncludeForAll.h"
#include"Weapon.h"//武器
#include"Camera.h"//カメラ
#include"EffectPoint.h"//エフェクト生成器
#include"ShipData.h"
#include"Engine.h"//エンジン
#include"EffectTemplate.h"//エフェクト生成器初期化容器
#include"SoundLoader.h"//音声ローダー
#include"FireController.h"//射撃管理
#include"Lock.h"//ロックする時に関わる操作やデータ
#include<stdio.h>
#include<stdlib.h>
class ShipMain :
	public AllMovableObjects
{
public:
	ShipMain() : AllMovableObjects(false, false, false, true) {
		currentEngineOutPutRate = 0;
		returnToCenter = false;

		forecastSeconds = 0;
		reviseRadianOnY = 0;
		reviseRadianOnZ = 0;

		/*テスト部分*/
		hitPoint = 240;
		shipMainCrash = { 180, 40, 18 };
		controled = false;
		canIShoot = false;

		flameAfterSink = 0;
	}
	~ShipMain();

	//初期化
	void RegistrateShipCrashParts();
	void InifThisShip(int *ShipHandle, int *SShadowH, int ShipNum,
		EffectTemplate ET, SoundLoader *SL);

	bool InifThisShip(PictureLoader *PL, EffectTemplate ET,
		SoundLoader *SL, int num, int SN);
	void SetThisShipPos(Coordinate<double> Coord, double radian);

	void SetEffectPoint(ShipData SD);
	void DestroyMemory();
	void SetWeaponTest(PictureLoader *PL);
	void SetWeapon();

	//沈む演出関連
	bool ReferSinkingEnding() { return flameAfterSink == sinkingFlame; }
	void countSinkingFlame() { flameAfterSink++; }

	//データ操作関数
	void SetSerialNumber(int x) { serialNumber = x; }

	//コントロール
	void ControlThisShip(int Command);

	//更新
	void Update();

	void SetControled() { controled = !controled; }

	//移動関連
	void SetEngineOutPutRate(bool up);
	void SetEngineOutPutRate(double oP) {
		currentEngineOutPutRate = oP;
		mainEngine.SetOutPutPercentage(currentEngineOutPutRate);//今の出力を与える
	}
	void CalSpeed();
	void ChangeDirect(bool right);
	void ReturnDirectChange() { returnToCenter = !returnToCenter; }
	void Alignment();

	void SetChangingDirect(double rad) {
		currentRadian = rad;
		if (rad > maxRadian)
			currentRadian = maxRadian;
		if (rad < -maxRadian)
			currentRadian = -maxRadian;
		SetRadianChangePerFrame(currentRadian);
	}

	void TEST();
	void TestDraw(double x, double z);

	/*簡単化した戦闘*/
	void SufferDamage(int damage);
	/*ロックオンの状態の操作*/
	void ChangeForecastSecond(bool up);
	void ChangeReviseRadianOnY(bool up);
	void ChangeReviseRadianOnZ(bool right);
	void ResetReviseData() {
		forecastSeconds = 0;
		reviseRadianOnY = 0;
	}

	//問い合わせ
	int ReferShipType() { return shipType; }
	const char * ReferName() { return name; }
	int ReferMaxHP() { return maxHitPoint; }
	int ReferHP() { return hitPoint; }
	/*移動関連*/
	bool ReferReturnOn() { return returnToCenter; }
	double ReferChangingRadian() { return currentRadian; }
	double ReferOutPutRate() { return currentEngineOutPutRate; }
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
	double ReferMainMaxRange() { return fireControllerMain.ReferMaxRange(); }
	int ReferDamage() { return MainWeapon[0].ReferDamage(); }

	/*collision関連*/
	double ReferShipCrashR() { return shipCrashR; }
	CrashSize3d<double> ReferShipCrashSize() { return shipMainCrash; }
	int ReferCollisionPointAmount() { return pointCount; }
	Coordinate2D<double> ReferCollisionPoint(int num) {
		return aroundPointPos[num];
	}
	void ResetStatus() {
		currentRadian = 0;
		currentEngineOutPutRate = 0;
		mainEngine.SetOutPutPercentage(currentEngineOutPutRate);
	}

	bool ReferControled() { return controled; }
	/*番号*/
	int ReferSerialNumber() { return serialNumber; }
	//エンジン関連
	double ReferEngineOutput() { return mainEngine.ReferOutput(); }
	double ReferEngineMaxOutput() { return mainEngine.ReferMaxOutput(); }
	/*ロック関係*/
	int ReferForecastSecond() { return forecastSeconds; }
	bool ReferCanIShoot() { return canIShoot; }

	//エフェクト生成
	Effect NewBubble(int num);
	Effect NewSmoke(int num);
	int ReferBubblePointCount() { return bubblePointCount; }
	int ReferSmokePointCount() { return smokePointCount; }

	void CheckAndPlaySound();

	/*射撃*/
	int HowManyWeaponYouHave() { return MainWeaponCount; }
	bool IsThisOneUsable(int Num, bool Main);
	Ammo Shoot(int Num, bool Main);
	void ShowMePointOfImpact(Camera camera);


	void LockAndAim(ShipMain *ship,bool render);

	/*ロック*/
	Lock fireDataFigureUp;

private:
	ShipCrashParts * MainParts;
	ShipCrashParts * SubParts;
	double shipCrashR;
	CrashSize3d<double> shipMainCrash;
	/*自分の弾が自分に当たらないように使用する番号です*/
	int serialNumber;
	int shipType;
	bool controled;

	Weapon * MainWeapon;//メイン武器
	int MainWeaponCount;//メイン武器の数
	Weapon * SubWeapon;//サブ武器
	int SubWeaponCount;//サブ武器の数

	EffectPoint * bubbleStartPoint;//水泡生成ポイント
	int bubblePointCount;//水泡生成ポイントの数

	/*あたり判定・泡生成用点*/
	Coordinate2D<double> aroundPointPos[AUTO_SHIP_DATA::AROUND_POINT];
	int pointCount;//エフェクト発生ポイントの数
	void FindThosePoint();

	EffectPoint * smokeStartPoint;
	int smokePointCount;//雲発生ポイントの数

	void MemorySecure();

	/*エンジン*/
	Engine mainEngine;
	double currentEngineOutPutRate;//出力のパーセンテージ

	/*砲のステータス変更*/
	bool TurnMainWeapon(bool right);
	bool PullMainWeapon(bool up);

	/*射撃管理*/
	void CalThePoint();
	void CalMainPoint();

	void DrawMainPoint(Camera camera);
	void DrawSubPoint(Camera camera);
	
	bool canIShoot;

	double draft;//喫水:魚雷を使う時に使うデータです。
	int thisShipType;

	int *shipHandle[SerialNumberOfShipHandle::ARRAY_SIZE];

	//方向舵
	double maxRadian;
	double currentRadian;
	double radianChangePerFrame;
	bool returnToCenter;
	
	char name[100];

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
	int maxHitPoint;

	/*射撃*/
	FireController fireControllerMain;
	FireController fireControllerSub;
	double targetRadianForMain;
	double targetRadianForSub;
	const int framesCountInaSecond = 60;//毎秒フレーム数
	const int maxForecastSecond = 15;
	int forecastSeconds;//何秒後の位置を推測したい
	Coordinate2D<double> nextPos;
	double reviseRadianOnY;
	double reviseRadianOnZ;
	const double maxreviseRadianOnY = MathAndPhysics::PI * (15.0 / 180.0);
	const double maxReviseRadianOnZ = MathAndPhysics::OneDegreeRadian * 30;

	void CalNextPos(ShipMain *ship);
	void CalDistance(ShipMain *ship);

	double distance;
	double targetRadianOnZforMain;

	const int sinkingFlame = 360;
	int flameAfterSink;

	const float lockCircleR = 70;
	const int lockCirclePosNum = 70;
	const float lockCircleThick = 2.0;

};