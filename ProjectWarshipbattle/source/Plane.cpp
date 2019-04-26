#include "Plane.h"

Plane::~Plane()
{
}
/*飛行機の状態におおじて移動する*/
void Plane::CalSpeed() {
	switch (status) {
	case FLYING:break;
	case FIGHTING:break;
	case TAKINGOFF:break;
	case LANDING:break;
	case FALLING:break;
	}
}

/*最大速度、重量、出力を設定する*/
void Plane::SetMSWeightPower(double Ms, double W, double P) {
	maxSpeed = Ms;
	weight = W;
	power = P;
	acceleratedSpeed = power / weight;
}

void Plane::TakeOff(double X, double Z, double R) {
	NewCoordX(X);
	NewCoordZ(Z);
	SetRadianOnZ(R);
	takedOff = true;
	status = TAKINGOFF;
}