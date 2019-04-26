#include "EffectPoint.h"

EffectPoint::EffectPoint()
{
}

EffectPoint::~EffectPoint()
{
}

Effect EffectPoint::NewEffect(double Radian, double Speed, 
	double CoordX, double CoordZ) {
	double newX = relativeCoordX;
	double newZ = relativeCoordZ;
	double realX;
	double realZ;
	/*���ۂɐ������ׂ��Ƃ���̍��W���v�Z����*/
	realX = CoordX - cos(Radian) * newX + sin(Radian) * newZ;
	realZ = CoordZ - cos(Radian) * newZ - sin(Radian) * newX;
	/*�G�t�F�N�g�|�C���g�ɕۑ������f�[�^�𗘗p���ĐV�����G�t�F�N�g�𐶐�����*/
	Effect E(movable,continueTime,Radian,Radian,0,Speed,realX,realZ,graphHandle,
		spread,zoomMutliple, zoomRate);
	return E;//�������X�e�[�^�X�������Ă���G�t�F�N�g��Ԃ�
}

/*���΍��W�A�ړ��ł��邩�A�p�����ԁA�ő吶�����A�g�U�ł��邩�A�g�嗦�A�g�嗦�̊g�嗦*/
void EffectPoint::InifCoordinate(double RCX, double RCZ, bool MA, int CT,
	int MACPF,bool Spread, double ZoomMutliple,double ZoomRate) {
	relativeCoordX = RCX;
	relativeCoordZ = RCZ;
	movable = MA;
	continueTime = CT;
	maxAmountCreatePerFrame = MACPF;
	spread = Spread;
	zoomMutliple = ZoomMutliple;
	zoomRate = ZoomRate;
}

/*������*/
void EffectPoint::Inif(int ET, int *Handle) {
	effectType = ET;//�G�t�F�N�g�^�C�v
	graphHandle = Handle;//�摜�n���h��
}

/*�f�o�b�O���[�h�I���ɂ��鎞�ɃG�t�F�N�g�̐����ꏊ��`�悷��*/
void EffectPoint::TestDraw(double r, double x, double z,double CX,double CZ) {
	unsigned int Cr = GetColor(255, 0, 0);

	double newX = relativeCoordX;
	double newZ = relativeCoordZ;
	double realX;
	double realZ;
	/*�{���̍��W���v�Z����*/
	realX = cos(r) * newX - sin(r) * newZ + x;
	realZ = cos(r) * newZ + sin(r) * newX + z;

	DrawPixel((int)(realX - CX), (int)(realZ - CZ), Cr);
}