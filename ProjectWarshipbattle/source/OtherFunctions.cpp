#include "OtherFunctions.h"

double Distance3D(Coordinate<double> A, Coordinate<double> B) {
	double Distance;
	Distance = sqrt(pow(A.x - B.x, 2) + pow(A.y - B.y, 2) + pow(A.z - B.z, 2));
	return Distance;
}

bool crash3DtoPoint(Coordinate<double>A, Coordinate<double> B,
	CrashSize3d<double> ACrash, double radian) {
	double aMinX, aMinY, aMinZ;
	double aMaxX, aMaxY, aMaxZ;
	double BX, BZ, BY;
	//�����̉~�̒��S���W���v�Z����
	BX = cos(radian)*(B.x - A.x) - sin(radian)*(B.y - A.y) + A.x;
	BZ = sin(radian)*(B.z - A.z) + cos(radian)*(B.y - A.y) + A.y;

	//���͈̂̔͂��v�Z����
	aMinX = A.x - ACrash.x / 2;
	aMaxX = A.x + ACrash.x / 2;
	aMinY = A.y;
	aMaxY = A.y + ACrash.y;
	aMinZ = A.z - ACrash.z / 2;
	aMaxZ = A.z + ACrash.z / 2;

	//�͈͓��ɓ�������true��Ԃ�
	if (B.x >= aMinX && B.x <= aMaxX &&
		B.y >= aMinY && B.y <= aMaxY &&
		B.z >= aMinZ && B.z <= aMaxZ) {
		return true;
	}
	//����ȊO��false��Ԃ�
	return false;
}