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
	//ù‰ñŒã‚Ì‰~‚Ì’†SÀ•W‚ğŒvZ‚·‚é
	BX = cos(-radian)*(B.x) - sin(-radian)*(B.z);
	BZ = sin(-radian)*(B.x) + cos(-radian)*(B.z);

	//À‘Ì‚Ì”ÍˆÍ‚ğŒvZ‚·‚é
	//ÄŒŸØ•K—v‚ª‚ ‚é
	double tempX, tempZ;
	tempX = cos(-radian) * A.x - sin(-radian) * A.z;//ù‰ñŒã‚Ì’†SÀ•W‚ğŒvZ‚·‚é
	tempZ = sin(-radian) * A.x + cos(-radian) * A.z;

	//” ‚Ì”ÍˆÍ‚ğŒvZ‚·‚é
	aMinX = tempX - ACrash.x / 2;
	aMinZ = tempZ - ACrash.z / 2;
	aMaxX = tempX + ACrash.x / 2;
	aMaxZ = tempZ + ACrash.z / 2;

	aMinY = A.y;
	aMaxY = A.y + ACrash.y;

	//” ‚É“ü‚Á‚½‚çtrue‚ğ•Ô‚·
	if (BX >= aMinX && BX <= aMaxX &&
		B.y >= aMinY && B.y <= aMaxY &&
		BZ >= aMinZ && BZ <= aMaxZ) {
		return true;
	}
	//‚»‚êˆÈŠO‚Ífalse‚ğ•Ô‚·
	return false;
}

bool crash3Dto3D(Coordinate<double>A, Coordinate<double> B,
	CrashSize3d<double> ACrash, CrashSize3d<double> BCrash,
	double radianA, double radianB) {
	return false;
}

double CalRadianBetweenPoints(Coordinate2D<double> A, Coordinate2D<double> B,
	double radian) {
	double targetRadianOnZforMain;

	//‹tOŠpŠÖ”‚ğ—˜—p‚µ‚Äƒ‰ƒWƒAƒ“‚ğŒvZ‚·‚é
	targetRadianOnZforMain = atan((B.z - A.z) / (B.x - A.x));

	if (A.x > B.x && A.z < B.z) {//“G‚Í‘æˆêÛŒÀ‚É‚¢‚é
		targetRadianOnZforMain = targetRadianOnZforMain;
	}
	else if (A.x > B.x && A.z > B.z) {//“G‚Í‘æ“ñÛŒÀ‚É‚¢‚é
		targetRadianOnZforMain = targetRadianOnZforMain;
	}
	else if (A.x < B.x && A.z > B.z) {//“G‚Í‘æOÛŒÀ‚É‚¢‚é
		targetRadianOnZforMain = targetRadianOnZforMain + MathAndPhysics::PI;
	}
	else {//“G‚Í‘ælÛŒÀ‚É‚¢‚é
		targetRadianOnZforMain = MathAndPhysics::PI + targetRadianOnZforMain;
	}

	/*…•½*/
	double difference = radian -
		targetRadianOnZforMain;
	//‚¢‚Â‚àù‰ñŠp“x‚ª¬‚³‚¢‚È•ûŒü‚Ö‰ñ‚·
	difference = fmod(difference, MathAndPhysics::PI);

	return difference;
}