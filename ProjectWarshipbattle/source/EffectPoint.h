#pragma once
#include"Effect.h"
#include"DxLib.h"

class EffectPoint
{
public:
	EffectPoint();
	~EffectPoint();
	
	Effect NewEffect(double Radian, double Speed,
		double CoordX, double CoordZ);

	/*���΍��W�A�ړ��ł��邩�A�p�����ԁA�ő吶�����A�g�U�ł��邩�A�g�嗦�A�g�嗦�̊g�嗦*/
	void InifCoordinate(double RCX,double RCZ, bool MA, int CT,int MACPF,
		bool Spread,double ZoomMutliple,double ZoomRate,int Type);

	void Inif(int ET, int *Handle);
	void TestDraw(double r, double s, double x, double CX, double CZ);

private:
	double relativeCoordX;
	double relativeCoordZ;
	
	int *graphHandle;//�G�t�F�N�g�̉摜�n���h����ۑ�����

	int effectType;//���̃G�t�F�N�g�����_�͉��𐶐�����
	int maxAmountCreatePerFrame;

	bool movable;//�ړ��ł��邩
	int continueTime;//�p������

	bool spread;//�g�U�ł��邩
	double zoomMutliple;
	double zoomRate;

	int type;
};

