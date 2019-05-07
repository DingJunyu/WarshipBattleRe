#pragma once
#include"DefinedData.h"
#include"AI_Action.h"
#include"ShipMain.h"
#include"IncludeForAll.h"
#include"OtherFunctions.h"
#include<cmath>

class FlagShipAI
{
public:
	FlagShipAI() :inBattle(false) {}
	~FlagShipAI();

	void LetUsGo(ShipMain *me, ShipMain *target);//移動関数-ステータスによって決めた行動を行う
	void ShowMeTheTarget();//敵を指定する関数

	double ReferRadianNeededNow() { return radianNeededNow; }
	double ReferSpeedInNeed() { return targetSpeedRate; }

private:
	/*状態部分*/
	bool inBattle;
	bool concentrateOn;/*一つのターゲットに集中するか*/

	/******************移動部分*****************/
	/*定数部分*/
	const double nextPosOnMapX = 1500;
	const double nextPosOnMapZ = 1500;
	const int randRange = 1000;
	const int nextPointFrame = 1800;//18秒後の位置をターゲットに設定する
	const double outPut_50 = 0.5;
	const double outPut_75 = 0.75;
	const double outPut_100 = 1.0;
	const int nextCounter = 600;
	/*関数部分*/
	void CalData(ShipMain *ship);
	void SetWayPoint();//ウェイポイント設定関数
	void SetWayPoint(Coordinate2D<double>, double radian, double speed);
	void DisableWayPoint_MoveWithEnemy(double radian);
	void SetNowRadian(double rad) { nowRadian = rad; }
	void SetMyPos(Coordinate2D<double> pos) { myPos = pos; }
	void CalTargetRadian();//敵との角度を計算して
	void SetRadianNeeded();// 今が必要な角度を設定する
	void CalDistance();//
	double SpeedINeed(double outPutRate);//出力を設定する
	/*変数部分*/
	Coordinate2D<double> wayPoint;//ウェイポイント
	Coordinate2D<double> myPos;//今自分の座標
	double targetRadian;//ウェイポイントに向く角度
	double nowRadian;//今自分の角度
	double radianNeededNow;//回転角度//targetRadianに近づけば近づいほど小さくなる
	double nowSpeed;//今自分の速度
	double targetSpeedRate;
	double distance;//ウェイポイントとの距離
	double targetDistance;//敵との距離
	const double needToChange = 300;
	int counter;

	/*艦隊の接触状態による*/
	double targetSpeed;
	double minMaxSpeedInFleet;//艦隊の中に一番遅い船の最大速度

	/*計算部分*/
	void CalRadianNeeded();//ウェイポイントへ行くため必要な角度の差
	void CalDistanceToOpponent();//相手との距離を計算する

	/*戦闘部分*/
	void ChooseTarget();//敵を選ぶ
	const double detectDistance = 2000;

};

