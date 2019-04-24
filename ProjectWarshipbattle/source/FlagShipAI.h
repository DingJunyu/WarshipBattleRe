#pragma once
#include"DefinedData.h"
#include"AI_Action.h"
#include"IncludeForAll.h"
class FlagShipAI
{
public:
	FlagShipAI() :inBattle(false) {}
	~FlagShipAI();

	void LetUsGo();//移動関数-ステータスによって決めた行動を行う
	void LetUsFight();//戦闘関数

private:
	/*状態部分*/
	bool inBattle;

	/*移動部分*/
	const double nextPosOnMapX = 1500;
	const double nextPosOnMapZ = 1500;
	const int randRange = 1000;

	void SetMyPos(Coordinate2D<double>);
	void SetWayPoint();//ウェイポイント設定関数
	void SetWayPoint(Coordinate2D<double>);
	Coordinate2D<double> wayPoint;
	Coordinate2D<double> myPos;
	double targetRadian;//ウェイポイントに向く角度
	double nowRadian;
	double radianNeededNow;//回転角度//targetRadianに近づけば近づいほど小さくなる

	/*艦隊の接触状態による*/
	double targetSpeed;
	double minMaxSpeedInFleet;//艦隊の中に一番遅い船の最大速度

	/*計算部分*/
	void CalRadianNeeded();//ウェイポイントへ行くため必要な角度の差
	void CalDistanceToOpponent();//相手との距離を計算する

	/*戦闘部分*/
	void ChooseTarget();//敵を選ぶ

};

