#include "Lock.h"

Lock::Lock()
{
	correction = { 0,0 };
	lockOn = false;
	targetNumber = 0;
}

Lock::~Lock()
{
}

/*予測時間を変更する関数*/
void Lock::Set_IntialLead_Seconds(bool up) {
	initialLead_Seconds = up ? +1 : -1;
}

/*予測距離を変更する関数*/
void Lock::ChangeCorrection_Distance(bool up) {
	double changeRadian = (0.5 / 180.0) * MathAndPhysics::PI;
	correction.y = up ? +changeRadian : -changeRadian;
}

void Lock::CalNextPosition(Coordinate<double> coord, double speed, double radian) {
	double distance = initialLead_Seconds * 60/*フレーム数*/ * speed + coord.x;

	/*次の位置を計算する*/
	nextPosition.x = coord.x + distance * cos(radian);
	nextPosition.z = coord.z + distance * sin(radian);
	nextPosition.y = coord.y;
}