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

void Ammo::Draw(Camera CM){
	double realX, realZ;
	/*自分野座標とカメラ座標を合わせて計算する*/
	realX = ReferCoordX() - CM.ReferRealCameraX();
	realZ = ReferCoordZ() - CM.ReferRealCameraZ();
	realX *= CM.ReferZoomRatio();
	realZ *= CM.ReferZoomRatio();

	/*画面に入る時だけ描画を行う*/
	if (realX > 0 && realX < Screen::SCREEN_X&&
		realZ>0 && realZ < Screen::SCREEN_Z) {
		DrawRotaGraph3((int)realX,
			(int)realZ,
			graphX / 2, graphZ / 2,
			multipleRate * CM.ReferZoomRatio(),
			multipleRate * CM.ReferZoomRatio(),
			ReferRadianOnZ(),
			*ammoHandle, TRUE, FALSE);
	}
}