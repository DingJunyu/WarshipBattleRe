#include "OtherFunctions.h"

double Distance3D(Coordinate<double> A, Coordinate<double> B) {
	double Distance;
	Distance = sqrt(pow(A.x - B.x, 2) + pow(A.y - B.y, 2) + pow(A.z - B.z, 2));
	return Distance;
}

double Distance2D(Coordinate<double> A, Coordinate<double> B) {
	return sqrt(pow(A.x - B.x, 2) + pow(A.z - B.z, 2));
}

double Distance2D(Coordinate2D<double> A, Coordinate2D<double> B) {
	return sqrt(pow(A.x - B.x, 2) + pow(A.z - B.z, 2));
}

bool crash3DtoPoint(Coordinate<double>A, Coordinate<double> B,
	CrashSize3d<double> ACrash, double radian) {
	double aMinX, aMinY, aMinZ;
	double aMaxX, aMaxY, aMaxZ;
	double BX, BZ;
	//旋回後の円の中心座標を計算する
	BX = cos(-radian)*(B.x) - sin(-radian)*(B.z);
	BZ = sin(-radian)*(B.x) + cos(-radian)*(B.z);

	//実体の範囲を計算する
	//再検証必要がある
	double tempX, tempZ;
	tempX = cos(-radian) * A.x - sin(-radian) * A.z;//旋回後の中心座標を計算する
	tempZ = sin(-radian) * A.x + cos(-radian) * A.z;

	//箱の範囲を計算する
	aMinX = tempX - ACrash.x / 2;
	aMinZ = tempZ - ACrash.z / 2;
	aMaxX = tempX + ACrash.x / 2;
	aMaxZ = tempZ + ACrash.z / 2;
	aMinY = A.y;
	aMaxY = A.y + ACrash.y;

	//箱に入ったらtrueを返す
	if (BX >= aMinX && BX <= aMaxX &&
		B.y >= aMinY && B.y <= aMaxY &&
		BZ >= aMinZ && BZ <= aMaxZ) {
		return true;
	}
	//それ以外はfalseを返す
	return false;
}

double CalRadianBetweenPoints(Coordinate2D<double> A, Coordinate2D<double> B,
	double radian) {
	double targetRadianOnZforMain;

	//逆三角関数を利用してラジアンを計算する
	targetRadianOnZforMain = atan((B.z - A.z) / (B.x - A.x));

	if (A.x > B.x && A.z < B.z) {//敵は第一象限にいる
		targetRadianOnZforMain = targetRadianOnZforMain;
	}
	else if (A.x > B.x && A.z > B.z) {//敵は第二象限にいる
		targetRadianOnZforMain = targetRadianOnZforMain;
	}
	else if (A.x < B.x && A.z > B.z) {//敵は第三象限にいる
		targetRadianOnZforMain = targetRadianOnZforMain + MathAndPhysics::PI;
	}
	else {//敵は第四象限にいる
		targetRadianOnZforMain = MathAndPhysics::PI + targetRadianOnZforMain;
	}

	/*水平*/
	double difference = radian -
		targetRadianOnZforMain;
	//いつも旋回角度が小さいな方向へ回す
	difference = fmod(difference, MathAndPhysics::PI);

	return difference;
}

/*座標予測関数*/
void NextPoint(Coordinate2D<double> *coord, double radian, double speed, int frames) {
	coord->x += (double)(speed * cos(radian) * frames);
	coord->z += (double)(speed * sin(radian) * frames);
}

/*ランダムポイント*/
void RandomPoint(Coordinate2D<double> *coord, double nextX, double nextZ,
	int randRange) {
	coord->x = nextX + (double)(rand() % randRange);
	coord->z = nextZ + (double)(rand() % randRange);
}

void NextPoint(Coordinate2D<double> *coord, double radian, double dis) {
	coord->x += cos(radian) * dis;
	coord->z += sin(radian) * dis;
}

void DrawString_Trans(
	int size, Coordinate2D<int> Coord, int trans,
	const char *str, unsigned int Cr){
	SetFontSize(size);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, trans);//透明度を下がる
	DrawString(Coord.x, Coord.z, str, Cr);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);//描画モードをもとに戻る
}

void DrawString_Trans(
	int size, Coordinate2D<int> Coord, int trans,
	int num, unsigned int Cr) {
	SetFontSize(size);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, trans);//透明度を下がる
	DrawFormatString(Coord.x, Coord.z, Cr, "%d", num);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);//描画モードをもとに戻る
}

void SetTrans(int trans) {
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, trans);
}

void SetTrans(long long trans) {
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)trans);
}

void ResetTrans() { 
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}