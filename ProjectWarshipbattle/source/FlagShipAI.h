#pragma once
#include"DefinedData.h"
#include"AI_Action.h"
#include"ShipMain.h"
#include"IncludeForAll.h"
#include"OtherFunctions.h"
#include<cmath>

enum RadianRange {
	RANGE_MAX = 60,
	RANGE_1_2 = 30,
	RANGE_1_4 = 10,
	RANGE_1_8 = 0,
	SPEED_MAX = 25,
	SPEED_1_2 = 18,
	SPEED_1_4 = 9,
	SPEED_1_8 = 4
};

enum DistanceRange {
	PATROL_RANGE = 3000,
	COMING_IN_RANGE = 2000,
	TAKE_T = 1000,
	BATTLE = 500
};

class FlagShipAI
{
public:
	FlagShipAI() :inBattle(false) {}
	~FlagShipAI();

	void LetUsGo(ShipMain *ship);//移動関数-ステータスによって決めた行動を行う
	void ShowMeTheTarget();//敵を指定する関数

	double ReferRadianNeededNow() { return radianNeededNow; }

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
	/*関数部分*/
	void CalData(ShipMain *ship);
	void SetWayPoint();//ウェイポイント設定関数
	void SetWayPoint(Coordinate2D<double>, double radian, double speed);
	void DisableWayPoint_MoveWithEnemy(double radian);
	void SetNowRadian(double rad) { nowRadian = rad; }
	void SetMyPos(Coordinate2D<double> pos) { myPos = pos; }
	void CalTargetRadianAndSetRadianNeeded();
	void CalDistance();
	/*変数部分*/
	Coordinate2D<double> wayPoint;//ウェイポイント
	Coordinate2D<double> myPos;//今自分の座標
	double targetRadian;//ウェイポイントに向く角度
	double nowRadian;//今自分の角度
	double radianNeededNow;//回転角度//targetRadianに近づけば近づいほど小さくなる
	double nowSpeed;//今自分の速度
	double distance;//ウェイポイントとの距離

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

