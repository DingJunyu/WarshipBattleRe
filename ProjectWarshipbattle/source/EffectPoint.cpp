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
	/*実際に生成すべきところの座標を計算する*/
	realX = CoordX - cos(Radian) * newX + sin(Radian) * newZ;
	realZ = CoordZ - cos(Radian) * newZ - sin(Radian) * newX;
	/*エフェクトポイントに保存したデータを利用して新しいエフェクトを生成する*/
	Effect E(movable,continueTime,Radian,Radian,0,Speed,realX,realZ,graphHandle,
		spread,zoomMutliple, zoomRate);
	return E;//正しいステータスを持っているエフェクトを返す
}

/*相対座標、移動できるか、継続時間、最大生成数、拡散できるか、拡大率、拡大率の拡大率*/
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

/*初期化*/
void EffectPoint::Inif(int ET, int *Handle) {
	effectType = ET;//エフェクトタイプ
	graphHandle = Handle;//画像ハンドル
}

/*デバッグモードオンにする時にエフェクトの生成場所を描画する*/
void EffectPoint::TestDraw(double r, double x, double z,double CX,double CZ) {
	unsigned int Cr = GetColor(255, 0, 0);

	double newX = relativeCoordX;
	double newZ = relativeCoordZ;
	double realX;
	double realZ;
	/*本当の座標を計算する*/
	realX = cos(r) * newX - sin(r) * newZ + x;
	realZ = cos(r) * newZ + sin(r) * newX + z;

	DrawPixel((int)(realX - CX), (int)(realZ - CZ), Cr);
}