#include "ArtificialIntelligence.h"
using namespace std;
/*AI*/
ArtificialIntelligence::ArtificialIntelligence()
{
	flagShip = false;
}

ArtificialIntelligence::~ArtificialIntelligence()
{
}

void ArtificialIntelligence::Move(ShipMain me, ShipMain target) {
	SetMyPos(me.ReferCoord2D_d());//位置を設定する
	SetNowRadian(me.ReferRadianOnZ());
	SetTargetPos(target.ReferCoord2D_d(), target.ReferRadianOnZ());//ウェイポイントを設置する
	CalWaypointDis();//距離を図る
	CalTargetRadian();//角度を計算する
	SetRadianNeeded();//必要な角度を計算する
	SetSpeed(target.ReferOutPutRate());//必要な速度を設置する
}

void ArtificialIntelligence::InBattle(ShipMain *me,
	vector<ShipMain> shipList,int targetNum) {
	double maxRange = me->ReferMainMaxRange();
	targetDis = MathAndPhysics::Infinite;//無限大に設置する
	double temp;
	int count = 0;//数をリセットする
	if (targetNum != -1) {
		me->fireDataFigureUp.SetNumber(targetNum);//ターゲットを設置する
	}
	else {
		for (auto ship = shipList.begin();
			ship != shipList.end();
			ship++) {
			if (ship->ReferAlive()) {
				temp = Distance2D(me->ReferCoord2D_d(), ship->ReferCoord2D_d());//距離を測る
				if (temp < targetDis) {//今の距離より小さければ
					targetDis = temp;//一番近い敵を選ぶ
					me->fireDataFigureUp.SetNumber(count);
				}			}
			count++;//順番を増やす
		}
	}
}

void ArtificialIntelligence::SetTargetPos(Coordinate2D<double> target,double radian) {
	wayPoint = target;//ウェイポイントを初期化
	NextPoint(&wayPoint, radian, disToFront);//角度と距離を利用してウェイポイントを更新します
}

void ArtificialIntelligence::CalTargetRadian() {
	targetRadian = CalRadianBetweenPoints(wayPoint, myPos, nowRadian);
}

void ArtificialIntelligence::SetRadianNeeded() {
	using namespace MathAndPhysics;
	/*範囲から今必要な角度を計算する*/
	if (abs(targetRadian) > RadianRange::RANGE_MAX * OneDegreeRadian) {
		radianNeededNow = RadianRange::SPEED_MAX * OneDegreeRadian;
	}
	if (abs(targetRadian) <= RadianRange::RANGE_MAX * OneDegreeRadian&&
		abs(targetRadian) > RadianRange::RANGE_1_2 * OneDegreeRadian) {
		radianNeededNow = RadianRange::SPEED_1_2 * OneDegreeRadian;
	}
	if (abs(targetRadian) <= RadianRange::RANGE_1_2 * OneDegreeRadian&&
		abs(targetRadian) > RadianRange::RANGE_1_4 * OneDegreeRadian) {
		radianNeededNow = RadianRange::SPEED_1_4 * OneDegreeRadian;
	}
	if (abs(targetRadian) <= RadianRange::RANGE_1_4 * OneDegreeRadian&&
		abs(targetRadian) > RadianRange::RANGE_1_8) {
		radianNeededNow = RadianRange::SPEED_1_8 * OneDegreeRadian;
	}
	if (targetRadian == RadianRange::RANGE_1_8) {
		radianNeededNow = 0;
	}

	/*方向を修正する*/
	if (targetRadian > 0)
		radianNeededNow = -radianNeededNow;
}

void ArtificialIntelligence::CalDistance(Coordinate2D<double> coord) {
	targetDis = Distance2D(coord, myPos);//目標距離を測る
}

void ArtificialIntelligence::CalWaypointDis() {
	wayPointDis = Distance2D(myPos, wayPoint);//ウェイポイントとの距離を測る
}

void ArtificialIntelligence::SetSpeed(double TGoutputRate) {
	/*ウェイポイントとの距離によって速度が変わります*/
	if (wayPointDis >= DistanceRange::CLOSING)
		outPutRate = 1;
	else if (wayPointDis <= DistanceRange::SLOW_DOWN
		&& wayPointDis > DistanceRange::STOP)
		outPutRate = 0.25;
	else if (wayPointDis <= DistanceRange::STOP)
		outPutRate = -0.3;
	else
		outPutRate = TGoutputRate - 0.05;
}