#include "Ammo.h"

Ammo::~Ammo()
{
}

void Ammo::CheckAlive() {
	//海に落ちたら使えないようにする
	if (ReferCoordY() <= -3) {
		isUsable = false;
	}
}

void Ammo::SetData() {
	/*画像のサイズを取る*/
	GetGraphSize(*ammoHandle, &graphX, &graphZ);
}

void Ammo::Draw(int x,int z){
	double realX, realZ;
	/*自分野座標とカメラ座標を合わせて計算する*/
	realX = ReferCoordX() - x;
	realZ = ReferCoordZ() - z;

	/*画面に入る時だけ描画を行う*/
	if (realX > 0 && realX < Screen::SCREEN_X&&
		realZ>0 && realZ < Screen::SCREEN_Z) {
		DrawRotaGraph3((int)ReferCoordX() - x,
			(int)ReferCoordZ() - z,
			graphX / 2, graphZ / 2,
			0.08, 0.08, ReferRadianOnZ(),
			*ammoHandle, TRUE, FALSE);
	}
}