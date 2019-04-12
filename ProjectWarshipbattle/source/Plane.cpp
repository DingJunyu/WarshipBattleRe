#include "Plane.h"

Plane::~Plane()
{
}

void Plane::CalSpeed() {
	switch (status) {
	case FLYING:break;
	case FIGHTING:break;
	case TAKINGOFF:break;
	case LANDING:break;
	case FALLING:break;
	}
}

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