#include "AllMovableObjects.h"

AllMovableObjects::~AllMovableObjects()
{
}

void AllMovableObjects::Move() {
	oldCoord = coord;
	SpeedDownbyAirResistance();//空気抵抗の影響で速度が落とす
	//2Dゲームですが、Y軸の移動も考える
	if (flyable) {//これは飛べるものか
		if (!plane) {//飛行機ではない場合
			FallingDown();//自由落下
		}
		coord.y += speedOnY;//Y座標の移動
	}

	if (!flyable) {
		//後退はまだやってない！
		if (input != 0) {
			if (abs(speedOnZ) < abs(maxSpeedOnZ * (input / maxInput))) {
				speedOnZ += input / mass * (1 - abs(speedOnZ / maxSpeedOnZ)) * 0.5;
			}
		}
		if (speedOnZ > 0																				) {
			if (speedOnZ > maxSpeedOnZ * (input / maxInput)) {
				speedOnZ -= (speedOnZ / maxSpeedOnZ > 0.5) ? 0.0015: 0.00075;
				//スピードが速いのほうが影響が高いようにしました。
				if (speedOnZ < 0)
					speedOnZ = 0;
			}
		}
		if (speedOnZ < 0) {
			if (speedOnZ < maxSpeedOnZ * (input / maxInput)) {
				speedOnZ += (speedOnZ / maxSpeedOnZ > 0.5) ? 0.0015 : 0.00075;
				//スピードが速いのほうが影響が高いようにしました。
				if (speedOnZ > 0)
					speedOnZ = 0;
			}
		}
	}

	/*速度がある場合に転向ができます*/
	if (speedOnZ != 0 && radianChangePerFrame != 0) {
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
}

void AllMovableObjects::Unmove() {
	coord = oldCoord;//前の座上に戻る
}

void AllMovableObjects::FallingDown() {//重力の影響で落下
	if (coord.z > 0 && flyable && !plane) {
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
	speedOnZLargerThan0 = speedOnZ > 0 ? true : false;
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
//自機描く時はこの関数を使う
//自機は画面の中心にいるため、ここはカメラの中心座標を使う
//次回更新について：DrawとDrawShadowに分ける、影の描画と本体の描画が別々に行います。
void AllMovableObjects::Draw(Camera CM) {
	double xOnScreen;
	double zOnScreen;
	//船から影への距離
	int shadowDistanceOnX = 2;
	int shadowDistanceOnZ = 2;
	//中心座標を取る
	xOnScreen = Screen::SCREEN_X / 2;
	zOnScreen = Screen::SCREEN_Z / 2;

	//影を描く
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 170);//透明度を下がる
	DrawRotaGraph3((int)xOnScreen + shadowDistanceOnX, 
		(int)zOnScreen + shadowDistanceOnZ, (int)(length / 2), 
		(int)(width) / 2, multiple, multiple,
		radianOnZ, *shadowHandle, TRUE, FALSE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);//描画モードをもとに戻る

	//本体を描く
	DrawRotaGraph3((int)xOnScreen, (int)zOnScreen, (int)(length / 2), 
		(int)(width) / 2, multiple, multiple,
		radianOnZ, *pictureHandle, TRUE, FALSE);

	/*テスト*/
	//unsigned int Cr = GetColor(255, 0, 0);
	//DrawCircleAA((float)xOnScreen,
	//	(float)zOnScreen, 100, 20, Cr, FALSE, 2.0f);
}

//自機以外のはこの関数を使う
//自機以外では自機に対して座標を利用して描く
void AllMovableObjects::DrawSub(Camera CM) {
	/*シャドーの距離計算はまだ*/
	int shadowDistanceOnX = 2;
	int shadowDistanceOnZ = 2;

	//カメラの座標を取って映す座標を計算する
	//影を描く
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 170);//透明度を下がる
	DrawRotaGraph3((int)(coord.x - CM.ReferRealCameraX() + shadowDistanceOnX),
		(int)(coord.z - CM.ReferRealCameraZ() + shadowDistanceOnZ),
		(int)(length / 2), (int)(width / 2),
		multiple, multiple, radianOnZ,
		*shadowHandle, TRUE, FALSE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);//描画モードをもとに戻る
	//本体を描く
	DrawRotaGraph3((int)(coord.x - CM.ReferRealCameraX()),
		(int)(coord.z - CM.ReferRealCameraZ()),
		(int)(length / 2), (int)(width / 2),
		multiple, multiple, radianOnZ,
		*pictureHandle, TRUE, FALSE);
}