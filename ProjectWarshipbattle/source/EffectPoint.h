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

	/*相対座標、移動できるか、継続時間、最大生成数、拡散できるか、拡大率、拡大率の拡大率*/
	void InifCoordinate(double RCX,double RCZ, bool MA, int CT,int MACPF,
		bool Spread,double ZoomMutliple,double ZoomRate,int Type);

	void Inif(int ET, int *Handle);
	void TestDraw(double r, double s, double x, double CX, double CZ);

private:
	double relativeCoordX;
	double relativeCoordZ;
	
	int *graphHandle;//エフェクトの画像ハンドルを保存する

	int effectType;//このエフェクト生成点は何を生成する
	int maxAmountCreatePerFrame;

	bool movable;//移動できるか
	int continueTime;//継続時間

	bool spread;//拡散できるか
	double zoomMutliple;
	double zoomRate;

	int type;
};

