#include "OtherFunctions.h"

double Distance3D(Coordinate<double> A, Coordinate<double> B) {
	double Distance;
	Distance = sqrt(pow(A.x - B.x, 2) + pow(A.y - B.y, 2) + pow(A.z - B.z, 2));
	return Distance;
}

double Distance2D(Coordinate<double> A, Coordinate<double> B) {
	return sqrt(pow(A.x - B.x, 2) + pow(A.z - B.z, 2));
}

bool crash3DtoPoint(Coordinate<double>A, Coordinate<double> B,
	CrashSize3d<double> ACrash, double radian) {
	double aMinX, aMinY, aMinZ;
	double aMaxX, aMaxY, aMaxZ;
	double BX, BZ;
	//�����̉~�̒��S���W���v�Z����
	BX = cos(-radian)*(B.x) - sin(-radian)*(B.z);
	BZ = sin(-radian)*(B.x) + cos(-radian)*(B.z);

	//���͈̂̔͂��v�Z����
	//�Č��ؕK�v������
	double tempX, tempZ;
	tempX = cos(-radian) * A.x - sin(-radian) * A.z;//�����̒��S���W���v�Z����
	tempZ = sin(-radian) * A.x + cos(-radian) * A.z;

	//���͈̔͂��v�Z����
	aMinX = tempX - ACrash.x / 2;
	aMinZ = tempZ - ACrash.z / 2;
	aMaxX = tempX + ACrash.x / 2;
	aMaxZ = tempZ + ACrash.z / 2;

	aMinY = A.y;
	aMaxY = A.y + ACrash.y;

	//���ɓ�������true��Ԃ�
	if (BX >= aMinX && BX <= aMaxX &&
		B.y >= aMinY && B.y <= aMaxY &&
		BZ >= aMinZ && BZ <= aMaxZ) {
		return true;
	}
	//����ȊO��false��Ԃ�
	return false;
}

bool crash3Dto3D(Coordinate<double>A, Coordinate<double> B,
	CrashSize3d<double> ACrash, CrashSize3d<double> BCrash,
	double radianA, double radianB) {
	return false;
}