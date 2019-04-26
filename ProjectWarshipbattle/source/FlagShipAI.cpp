#include "FlagShipAI.h"

FlagShipAI::~FlagShipAI()
{
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
	NextPoint(&wayPoint,radian, speed, nextPointFrame);
}

/*敵と近づくと敵の進行方向と同じ方向に進む*/
void FlagShipAI::DisableWayPoint_MoveWithEnemy(double radian) {
	targetRadian = radian;
}

/*目標との角度を計算する*/
void FlagShipAI::CalTargetRadianAndSetRadianNeeded() {
	using namespace MathAndPhysics;

	targetRadian = CalRadianBetweenPoints(wayPoint, myPos, nowRadian);

	/*範囲から今必要な角度を計算する*/
	if (abs(targetRadian) > RANGE_MAX * OneDegreeRadian) {
		radianNeededNow = SPEED_MAX * OneDegreeRadian;
	}
	if (abs(targetRadian) <= RANGE_MAX * OneDegreeRadian&&
		abs(targetRadian) > RANGE_1_2 * OneDegreeRadian) {
		radianNeededNow = SPEED_1_2 * OneDegreeRadian;
	}
	if (abs(targetRadian) <= RANGE_1_2 * OneDegreeRadian&&
		abs(targetRadian) > RANGE_1_4 * OneDegreeRadian) {
		radianNeededNow = SPEED_1_4 * OneDegreeRadian;
	}
	if (abs(targetRadian) <= RANGE_1_4 * OneDegreeRadian&&
		abs(targetRadian) > RANGE_1_8) {
		radianNeededNow = SPEED_1_8 * OneDegreeRadian;
	}
	if (targetRadian == RANGE_1_8) {
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
