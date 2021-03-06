#include "AllMovableObjects.h"

AllMovableObjects::~AllMovableObjects()
{
	pictureHandle = nullptr;
	shadowHandle = nullptr;	
}

void AllMovableObjects::Move() {
	oldCoord = coord;
	//2Dゲームですが、Y軸の移動も考える
	if (flyable) {//これは飛べるものか
		if (!plane) {//飛行機ではない場合
			FallingDown();//自由落下
			SpeedDownbyAirResistance();//空気抵抗で速度が落とす
		}
		coord.y += speedOnY;//Y座標の移動
	}

	if (input != 0) {//出力がある場合に速度を計算する
		/*加速の部分*/
		if (abs(speedOnZ) < abs(maxSpeedOnZ * (input / maxInput))) {
			speedOnZ += input / mass * (1 - abs(speedOnZ / maxSpeedOnZ)) * 0.5;
			/*速度は最大速度に近くになると加速度が落とす(重量と速度関係ある)*/
		}
	}

	/*出力が可能なものは速度が足らない場合に速度が落とす*/
	if (!flyable) {
		/*前進する場合*/
		if (speedOnZ > 0) {
			if (speedOnZ > maxSpeedOnZ * (input / maxInput)) {//もし速度は今の最大速度以上であれば
				speedOnZ -= (speedOnZ / maxSpeedOnZ > 0.5) ?
					HighSlowDownSpeed : LowSlowDownSpeed;//速度が早い時に減速も速い
				//スピードが速いのほうが影響が高いようにしました。
				if (speedOnZ < 0)//前進の時に後退にならないよう
					speedOnZ = 0;
			}
		}
		/*後退する場合*/
		if (speedOnZ < 0) {
			//もし速度は今の最大速度以上であれば
			if (speedOnZ < maxSpeedOnZ * (input / maxInput)) {
				//速度が早い時に減速も速い
				speedOnZ += (speedOnZ / maxSpeedOnZ > 0.5) ?
					HighSlowDownSpeed : LowSlowDownSpeed;
				//スピードが速いのほうが影響が高いようにしました。
				if (speedOnZ > 0)
					speedOnZ = 0;
			}
		}
	}

	/*速度がある場合に転向ができます*/
	if (speedOnZ != 0 && radianChangePerFrame != 0) {
		//速度が早ければ早いほど転向が早くなる
		radianOnZ += (radianChangePerFrame/100) * speedOnZ;
	}
	//オーバーフロー対策:Radianの範囲は-2π~2π
	if (radianOnZ > (MathAndPhysics::PI * 2)) {
		radianOnZ -= (MathAndPhysics::PI * 2);
	}
	if (radianOnZ < - (MathAndPhysics::PI * 2)) {
		radianOnZ += (MathAndPhysics::PI * 2);
	}

	//移動した座標を計算する
	coord.x += speedOnZ * cos(radianOnZ);
	coord.z += speedOnZ * sin(radianOnZ);

	distanceMoved += abs(speedOnZ);//正数を記録する
}

void AllMovableObjects::Unmove() {
	coord = oldCoord;//前の座上に戻る
}

void AllMovableObjects::FallingDown() {//重力の影響で落とす
	if (coord.y > 0 && flyable && !plane) {
		speedOnY -= MathAndPhysics::GRAVITATIONAL_ACCELERATION;
	}
}

void AllMovableObjects::SetSpeed(double Spe) {
	//水平面では垂直方向のスピードが要らない
	if (!flyable) {
		speedOnY = 0;
		speedOnZ = Spe;
	}
	else {//速度を三つの方向に分解する
		speedOnY = sin(radianOnY)*Spe;
		speedOnZ = cos(radianOnY)*Spe;
	}
	speedOnZLargerThan0 = speedOnZ > 0 ? true : false;//方向を設置する
}

void AllMovableObjects::SpeedDownbyAirResistance() {
	if (speedOnZ == 0)
		return;
	if (airResistance > MathAndPhysics::MIN_AIR_RESISTANCE)
		airResistance *= 0.99;//空気抵抗の影響がどんどん減らす
	else
		airResistance = MathAndPhysics::MIN_AIR_RESISTANCE;//限界に超えたら最少空気抵抗になる

	if (speedOnZLargerThan0 && speedOnZ > 0) {
		speedOnZ -= airResistance;//速度を減らす
		if (speedOnZ < 0)//方向が変えない
			speedOnZ = 0;
	}
	if (!speedOnZLargerThan0 && speedOnZ < 0) {
		speedOnZ += airResistance * 0.1;//速度を減らす
		if (speedOnZ > 0)//方向が変えない
			speedOnZ = 0;
	}
}

/*描画について：
分ける必要はないです*/

//自機描く時はこの関数を使う
//自機は画面の中心にいるため、ここはカメラの中心座標を使う
//次回更新について：DrawとDrawShadowに分ける、影の描画と本体の描画が別々に行います。
void AllMovableObjects::Draw(Camera CM) {
	double xOnScreen;
	double zOnScreen;
	//中心座標を取る
	xOnScreen = Screen::SCREEN_X / 2;
	zOnScreen = Screen::SCREEN_Z / 2;

	//本体を描く
	DrawRotaGraph3((int)xOnScreen, (int)zOnScreen, (int)(length / 2), 
		(int)(width) / 2, multiple, multiple,
		radianOnZ, *pictureHandle, TRUE, FALSE);
}

void AllMovableObjects::DrawShadow(Camera CM) {
	double xOnScreen;
	double zOnScreen;
	//船から影への距離
	int shadowDistanceOnX = 2;
	int shadowDistanceOnZ = 2;
	//中心座標を取る
	xOnScreen = Screen::SCREEN_X / 2;
	zOnScreen = Screen::SCREEN_Z / 2;

	//影を描く
	SetTrans(170);//透明度を下がる
	DrawRotaGraph3((int)xOnScreen + shadowDistanceOnX,
		(int)zOnScreen + shadowDistanceOnZ, (int)(length / 2),
		(int)(width) / 2, multiple, multiple,
		radianOnZ, *shadowHandle, TRUE, FALSE);
	ResetTrans();//描画モードをもとに戻る
}

//自機以外のはこの関数を使う
//自機以外では自機に対して座標を利用して描く
void AllMovableObjects::DrawSub(Camera CM) {
	/*自分の座標とカメラ座標を合わせて計算する*/
	double coordX = coord.x - CM.ReferRealCameraX();
	double coordZ = coord.z - CM.ReferRealCameraZ();
	coordX *= CM.ReferZoomRatio();
	coordZ *= CM.ReferZoomRatio();

	//本体を描く
	/*画面に入る時だけ描画を行う*/
	if (coordX > 0 - Screen::OutRange && coordX < Screen::SCREEN_X + Screen::OutRange
		&& coordZ > 0 - Screen::OutRange &&
		coordZ < Screen::SCREEN_Z + Screen::OutRange) {
		DrawRotaGraph3((int)(coordX),
			(int)(coordZ),
			(int)(length / 2), (int)(width / 2),
			multiple * CM.ReferZoomRatio(), 
			multiple * CM.ReferZoomRatio(), 
			radianOnZ,
			*pictureHandle, TRUE, FALSE);
	}
}

void AllMovableObjects::DrawSubShadow(Camera CM) {
	/*シャドーの距離計算はまだ*/
	int shadowDistanceOnX = 2;
	int shadowDistanceOnZ = 2;

	/*座標とカメラ座標を合わせて計算する*/
	double coordX = coord.x - CM.ReferRealCameraX();
	double coordZ = coord.z - CM.ReferRealCameraZ();
	coordX *= CM.ReferZoomRatio();
	coordZ *= CM.ReferZoomRatio();

	//カメラの座標を取って映す座標を計算する
	//影を描く
	/*画面に入る時だけ描画を行う*/
	if (coordX > 0 - Screen::OutRange && coordX < Screen::SCREEN_X + Screen::OutRange &&
		coordZ>0 - Screen::OutRange &&
		coordZ < Screen::SCREEN_Z + Screen::OutRange) {
		SetTrans(170);//透明度を下がる
		DrawRotaGraph3((int)(coordX) + shadowDistanceOnX,
			(int)(coordZ) + shadowDistanceOnZ,
			(int)(length / 2), (int)(width / 2),
			multiple * CM.ReferZoomRatio(),
			multiple * CM.ReferZoomRatio(), radianOnZ,
			*shadowHandle, TRUE, FALSE);
		ResetTrans();
	}//描画モードをもとに戻る
}

//編成画面に使わる船の画像を描画する
void AllMovableObjects::DrawCard(Coordinate2D<double> coord) {
	const double & multiple = MathAndPhysics::ShipCardRate;//規定された倍率を設定する

	DrawExtendGraph((int)coord.x, (int)coord.z,
		(int)(coord.x + multiple * length),
		(int)(coord.z + multiple * width),
		*pictureHandle, TRUE);
}