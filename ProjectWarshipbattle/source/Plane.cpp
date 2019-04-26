#include "Plane.h"

Plane::~Plane()
{
}
/*��s�@�̏�Ԃɂ������Ĉړ�����*/
void Plane::CalSpeed() {
	switch (status) {
	case FLYING:break;
	case FIGHTING:break;
	case TAKINGOFF:break;
	case LANDING:break;
	case FALLING:break;
	}
}

/*�ő呬�x�A�d�ʁA�o�͂�ݒ肷��*/
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