#include "Effect.h"

Effect::Effect() {

}

Effect::~Effect()
{
}

void Effect::Draw(int x,int z) {
	double rotateX = 0;
	double rotateZ = 0;
	double offsetOnX;
	double offsetOnZ;

	offsetOnX = zoomMutliple * zoomMutliple * graphX * 0.25 ;//Ｘ座標の偏移を計算
	offsetOnZ = zoomMutliple * zoomMutliple * graphZ * 0.25 ;//Ｚ座標の偏移を計算
	
	double realX, realZ;

	/*カメラの座標と偏移と今の座標を合わせて、書くべきところの座標を計算する*/
	realX = coordX - x - offsetOnX;
	realZ = coordZ - z - offsetOnZ;

//	if (type == TypeOfEffect::EXPLOSION||type==TypeOfEffect::RIPPLE||
//		type == TypeOfEffect::SMOKE) {
		nextFlame = flame % 64;
		nextFlame /= 4;
//	}

	/*画面に入る時だけ描画を行う*/
	if (realX > 0 && realX < Screen::SCREEN_X&&
		realZ>0 && realZ < Screen::SCREEN_Z) {
		DrawRotaGraph3((int)coordX - x - (int)offsetOnX,
			(int)coordZ - z - (int)offsetOnZ,
			0, 0,
			zoomMutliple, zoomMutliple, radian - MathAndPhysics::PI / 4,
			*(graphicHandle + nextFlame), TRUE, FALSE);
	}

	flame++;

	/*テスト用*/
	/*unsigned int Cr = GetColor(0, 0, 255);
	DrawPixel(coordX - x, coordZ - z, Cr);*/
}

void Effect::Check() {
	if (spread) {//拡散ができれば
		zoomMutliple *= zoomRate;
		if (zoomRate > 1)//拡大率がフレームごとに減る
			zoomRate *= 0.99;
		if (zoomRate <= 1)//最小にしても１です
			zoomRate = 1;
	}

	//目標角度になる前に回る
	if (radian != targetRadian && abs(radian - targetRadian) >
		radianChangePerFrame) {
		if (radian < 0)//0より小さい時は0に向かって旋回する
			radian += radianChangePerFrame;
		if (radian > 0)//0より大きい時は0に向かって旋回する
			radian -= radianChangePerFrame;
	}

	//このエフェクトの時間が切れたか
	if (GetNowCount() > endTime)
		timeUp = true;
}

void Effect::Move() {
	//移動する前に角度をランダムに変わる
	if (rand() % 2 == 0)//確率は50%
		radian += (double)((rand() % 2) / 180.0f) * MathAndPhysics::PI;
	else
		radian -= (double)((rand() % 2) / 180.0f) * MathAndPhysics::PI;

	//移動する
	coordX += cos(radian)*speed;
	coordZ += sin(radian)*speed;
}