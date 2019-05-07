#pragma once
#include"DefinedData.h"
#include"ShipMain.h"
#include"OtherFunctions.h"
#include"AI_Action.h"
#include"Ammo.h"
#include<vector>
#include<list>
/*このクラスにはＡＩの行動を実現する*/
class ArtificialIntelligence
{
public:
	ArtificialIntelligence();
	~ArtificialIntelligence();

	void SetStatus(ShipMain ship);
	void Move(ShipMain me,ShipMain target);//フラグシープ以外はフラグシープを目標にして移動する
	void InBattle(ShipMain *me,std::vector<ShipMain> shipList,
		int targetNum);

	void SetFlagShip() { flagShip = true; }//フラグシープフラグを設置
	bool ReferFlagShip() { return flagShip; }

	//船状態問い合わせ関数
	double ReferRadianNeededNow() { return radianNeededNow; }
	double ReferOutPutRateNeededNow() { return outPutRate; }

private:
	const double disToFront = 50;

	double range;//
	double targetDis;//目標との距離
	double wayPointDis;//ウェイポイントとの距離
	Coordinate2D<double> wayPoint;//ウェイポイント
	Coordinate2D<double> myPos;//自分の位置
	bool flagShip;//フラグシープフラグ
	double nowRadian;//今の角度
	double targetRadian;//目標角度
	double radianNeededNow;//転回角度
	double outPutRate;//速度

	void SetTargetPos(Coordinate2D<double> target,double radian);//ウェイポイント設置関数
	void SetMyPos(Coordinate2D<double> pos) { myPos = pos; }//自分の位置設置関数
	void SetNowRadian(double rad) { nowRadian = rad; }
	void CalTargetRadian();
	void SetRadianNeeded();
	void CalDistance(Coordinate2D<double> coord);
	void CalWaypointDis();
	void SetSpeed(double outputRate);
};

