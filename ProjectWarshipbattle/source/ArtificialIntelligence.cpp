#include "ArtificialIntelligence.h"
/*AI*/
ArtificialIntelligence::ArtificialIntelligence()
{
	flagShip = false;
}

ArtificialIntelligence::~ArtificialIntelligence()
{
}

void ArtificialIntelligence::Move(ShipMain me, ShipMain target) {
	SetMyPos(me.ReferCoord2D_d());
	SetNowRadian(me.ReferRadianOnZ());
	SetTargetPos(target.ReferCoord2D_d(), target.ReferRadianOnZ());
	CalWaypointDis();
	CalTargetRadian();
	SetRadianNeeded();
	SetSpeed(target.ReferOutPutRate());
}

void ArtificialIntelligence::SetTargetPos(Coordinate2D<double> target,double radian) {
	wayPoint = target;
	NextPoint(&wayPoint, radian, disToFront);
}

void ArtificialIntelligence::CalTargetRadian() {
	targetRadian = CalRadianBetweenPoints(wayPoint, myPos, nowRadian);
}

void ArtificialIntelligence::SetRadianNeeded() {
	using namespace MathAndPhysics;
	/*”ÍˆÍ‚©‚ç¡•K—v‚ÈŠp“x‚ðŒvŽZ‚·‚é*/
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

	/*•ûŒü‚ðC³‚·‚é*/
	if (targetRadian > 0)
		radianNeededNow = -radianNeededNow;
}

void ArtificialIntelligence::CalDistance(Coordinate2D<double> coord) {
	targetDis = Distance2D(coord, myPos);
}

void ArtificialIntelligence::CalWaypointDis() {
	wayPointDis = Distance2D(myPos, wayPoint);
}

void ArtificialIntelligence::SetSpeed(double TGoutputRate) {
	if (wayPointDis >= DistanceRange::CLOSING)
		outPutRate = 1.0;
	else if (wayPointDis <= DistanceRange::SLOW_DOWN
		&& wayPointDis > DistanceRange::STOP)
		outPutRate = 0.25;
	else if (wayPointDis <= DistanceRange::STOP)
		outPutRate = 0;
	else
		outPutRate = TGoutputRate;
}