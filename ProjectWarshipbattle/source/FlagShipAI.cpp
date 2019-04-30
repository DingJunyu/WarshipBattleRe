#include "FlagShipAI.h"

FlagShipAI::~FlagShipAI()
{
}

void FlagShipAI::LetUsGo(ShipMain *me, ShipMain *target) {
	counter %= nextCounter;
	counter++;

	SetMyPos(me->ReferCoord2D_d());//自分の位置を設定する
	SetNowRadian(me->ReferRadianOnZ());//自分の角度を設定する
	targetDistance = Distance2D(target->ReferCoord2D_d(), myPos);//目標との距離を測量する
	CalDistance();//前のウェイポイントとの距離を計算する
	if (targetDistance > (double)DistanceRange::BATTLE) {
		if (targetDistance > (double)DistanceRange::PATROL_RANGE) {
			if (distance < needToChange)//前のウェイポイントが使えなくなる時に
				SetWayPoint();//新しいウェイポイントを作る		
		}
		if (targetDistance <= (double)DistanceRange::COMING_IN_RANGE) {
			if (counter == 1) {
				SetWayPoint(target->ReferCoord2D_d(), target->ReferRadianOnZ(),
					target->ReferSpeedOnZ());
			}
		}
		CalTargetRadian(); 
	}
	else {
		DisableWayPoint_MoveWithEnemy(target->ReferRadianOnZ());
	}
	SetRadianNeeded();//角度を更新
	me->SetEngineOutPutRate(SpeedINeed(target->ReferOutPutRate()));
}

void FlagShipAI::SetWayPoint() {
	/*ランダムで次のターゲットを探す*/
	RandomPoint(&wayPoint,nextPosOnMapX, nextPosOnMapZ, randRange);

	if (myPos.x > 0 && myPos.z > 0) {
		int target = rand() % 3;
		switch (target) {
		case 0:wayPoint.ChangeX(); break;//第四象限に移す
		case 1:wayPoint.ChangeZ(); break;//第二象限に移す
		case 2:wayPoint.ChangeXandZ(); break;//第三象限に移す
		}
	}
}

/*敵の進行方向の前の点をターゲット点に設定する*/
void FlagShipAI::SetWayPoint(Coordinate2D<double> targetPos, double radian, double speed) {
	wayPoint = targetPos;
	if (speed == 0)
		speed = 0.1;
	NextPoint(&wayPoint,radian, speed, nextPointFrame);
}

/*敵と近づくと敵の進行方向と同じ方向に進む*/
void FlagShipAI::DisableWayPoint_MoveWithEnemy(double radian) {
	targetRadian = radian - nowRadian;
	targetRadian = fmod(targetRadian, MathAndPhysics::PI);
}

/*目標との角度を計算する*/
void FlagShipAI::CalTargetRadian() {
	targetRadian = CalRadianBetweenPoints(wayPoint, myPos, nowRadian);
}

void FlagShipAI::SetRadianNeeded() {
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
	if (targetRadian < 0)
		radianNeededNow = -radianNeededNow;
}

/*距離を計算する関数*/
void FlagShipAI::CalDistance() {
	distance = Distance2D(wayPoint, myPos);
}

double FlagShipAI::SpeedINeed(double outPutRate) {
	/*パトロール時の出力は半分*/
	if (targetDistance >= (double)DistanceRange::PATROL_RANGE) {
		return outPut_50;
	}
	/*接近する時は0.75*/
	if (targetDistance < (double)DistanceRange::COMING_IN_RANGE&&
		targetDistance >= (double)DistanceRange::TAKE_T) {
		return outPut_75;
	}
	/*前の位置を取る時は全速*/
	if (targetDistance < (double)DistanceRange::TAKE_T&&
		targetDistance >= (double)DistanceRange::BATTLE) {
		return outPut_100;
	}
	/*戦闘の時は敵の速度に合わせる*/
	if (targetDistance < (double)DistanceRange::BATTLE) {
		return outPutRate;
	}
}