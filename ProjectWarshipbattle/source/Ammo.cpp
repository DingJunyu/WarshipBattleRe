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
	GetGraphSize(*ammoHandle, &graphX, &graphZ);
}

void Ammo::Draw(int x,int z){
	double realX, realZ;
	realX = ReferCoordX() - x;
	realZ = ReferCoordZ() - z;

	/*画面に入る時だけ描画を行う*/
	if (realX > 0 && realX < Screen::SCREEN_X&&
		realZ>0 && realZ < Screen::SCREEN_Z) {
		DrawRotaGraph3((int)ReferCoordX() - x,
			(int)ReferCoordZ() - z,
			graphX / 2, graphZ / 2,
			0.05, 0.05, ReferRadianOnZ(),
			*ammoHandle, TRUE, FALSE);
	}
}