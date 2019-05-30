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

double CalRadianBetweenPoints(Coordinate2D<double> A, Coordinate2D<double> B,
	double radian) {
	double targetRadianOnZforMain;

	//�t�O�p�֐��𗘗p���ă��W�A�����v�Z����
	targetRadianOnZforMain = atan((B.z - A.z) / (B.x - A.x));

	if (A.x > B.x && A.z < B.z) {//�G�͑��ی��ɂ���
		targetRadianOnZforMain = targetRadianOnZforMain;
	}
	else if (A.x > B.x && A.z > B.z) {//�G�͑��ی��ɂ���
		targetRadianOnZforMain = targetRadianOnZforMain;
	}
	else if (A.x < B.x && A.z > B.z) {//�G�͑�O�ی��ɂ���
		targetRadianOnZforMain = targetRadianOnZforMain + MathAndPhysics::PI;
	}
	else {//�G�͑�l�ی��ɂ���
		targetRadianOnZforMain = MathAndPhysics::PI + targetRadianOnZforMain;
	}

	/*����*/
	double difference = radian -
		targetRadianOnZforMain;
	//��������p�x���������ȕ����։�
	difference = fmod(difference, MathAndPhysics::PI);

	return difference;
}

/*���W�\���֐�*/
void NextPoint(Coordinate2D<double> *coord, double radian, double speed, int frames) {
	coord->x += (double)(speed * cos(radian) * frames);
	coord->z += (double)(speed * sin(radian) * frames);
}

/*�����_���|�C���g*/
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
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, trans);//�����x��������
	DrawString(Coord.x, Coord.z, str, Cr);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);//�`�惂�[�h�����Ƃɖ߂�
}

void DrawString_Trans(
	int size, Coordinate2D<int> Coord, int trans,
	int num, unsigned int Cr) {
	SetFontSize(size);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, trans);//�����x��������
	DrawFormatString(Coord.x, Coord.z, Cr, "%d", num);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);//�`�惂�[�h�����Ƃɖ߂�
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

void DrawLoading_All(int num, int AsyncLoadNum) {
	ClearDrawScreen();

	unsigned int Cr;
	Cr = GetColor(255, 255, 255);
	DxLib::SetFontSize(30);

	DrawString((int)(0.8*Screen::SCREEN_X),
		(int)(0.8*Screen::SCREEN_Z), "LOADING...", Cr);
	DrawFormatString((int)(0.8*Screen::SCREEN_X),
		(int)(0.85*Screen::SCREEN_Z), Cr, "%d/%d", num, AsyncLoadNum);
	DrawBox(0, (int)(0.9*Screen::SCREEN_Z),
		(int)((double)num / (double)AsyncLoadNum * Screen::SCREEN_X),
		(int)(0.95*Screen::SCREEN_Z),
		Cr, TRUE);

	DxLib::ScreenFlip();
}

bool DrawLoad_All(int AsyncLoadNum, LONGLONG StartTime) {
	while (ProcessMessage() == 0) {
		DrawLoading_All(AsyncLoadNum - GetASyncLoadNum(), AsyncLoadNum);
		if (GetASyncLoadNum() == 0)
			break;
		if (GetTickCount() - StartTime > TIME_NEEDED::ONE_MINUTE) {
			return false;
		}
	}
	return true;
}