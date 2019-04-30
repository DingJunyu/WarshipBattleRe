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
	SetTargetPos(target.ReferCoord2D_d(), target.ReferRadianOnZ());
	CalTargetRadian();
	SetRadianNeeded();
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
	/*îÕàÕÇ©ÇÁç°ïKóvÇ»äpìxÇåvéZÇ∑ÇÈ*/
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

	/*ï˚å¸ÇèCê≥Ç∑ÇÈ*/
	if (targetRadian < 0)
		radianNeededNow = -radianNeededNow;
}

void ArtificialIntelligence::CalDistance(Coordinate2D<double> coord) {
	targetDis = Distance2D(coord, myPos);
}

void ArtificialIntelligence::CalWaypointDis() {
	wayPointDis = Distance2D(myPos, wayPoint);
}