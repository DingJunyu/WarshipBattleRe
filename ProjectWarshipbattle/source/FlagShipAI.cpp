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

/*目標との角度を計算する*/
void FlagShipAI::CalTargetRadian() {
	targetRadian = CalRadianBetweenPoints(wayPoint, myPos, nowRadian);
}