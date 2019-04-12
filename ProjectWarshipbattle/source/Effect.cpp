#include "Effect.h"

Effect::Effect() {

}

Effect::~Effect()
{
}

void Effect::Draw(int x,int z) {
	if (spread) {//拡散ができれば
		zoomMutliple *= zoomRate;
		if (zoomRate > 1)//拡大率がフレームごとに減る
			zoomRate *= 0.99;
		if (zoomRate <= 1)
			zoomRate = 1;
	}

	double rotateX = 0;
	double rotateZ = 0;
	double offsetOnX;
	double offsetOnZ;

	offsetOnX = zoomMutliple * zoomMutliple * graphX * 0.25 ;//Ｘ座標の偏移を計算
	offsetOnZ = zoomMutliple * zoomMutliple * graphZ * 0.25 ;//Ｚ座標の偏移を計算

	DrawRotaGraph3((int)coordX - x - (int)offsetOnX,
		(int)coordZ - z - (int)offsetOnZ,
		0, 0,
		zoomMutliple, zoomMutliple, radian - MathAndPhysics::PI/4,
		*graphicHandle, TRUE, FALSE);

	/*テスト用*/
	/*unsigned int Cr = GetColor(0, 0, 255);
	DrawPixel(coordX - x, coordZ - z, Cr);*/

	//目標角度になる前に回る
	if (radian != targetRadian && abs(radian - targetRadian) > 
		radianChangePerFrame) {
		if (radian < 0)
			radian += radianChangePerFrame;
		if (radian > 0)
			radian -= radianChangePerFrame;
	}
}

void Effect::Check() {
	//このエフェクトの時間が切れたか
	if (GetNowCount() > endTime)
		timeUp = true;
}

void Effect::Move() {
	//移動する前に角度をランダムに変わる
	if (rand()%2==0)
		radian += (double)((rand() % 2) / 180.0f) * MathAndPhysics::PI;
	else
		radian -= (double)((rand() % 2) / 180.0f) * MathAndPhysics::PI;

	coordX += cos(radian)*speed;
	coordZ += sin(radian)*speed;
}