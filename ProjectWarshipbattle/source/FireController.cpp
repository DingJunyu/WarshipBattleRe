#include "FireController.h"

FireController::FireController() :testAmmo(true, false, true, true)
{
}

FireController::~FireController()
{
}

/*初期化する時必ずこの関数を呼び出す*/
void FireController::InifDistance(double MaxRadian, double MinRadian) {
	/*角度の範囲です*/
	/*最大角度と最小角度はすべて正数と予想できるが、
	負数になる可能性もゼロのわけではないので、
	デバッグの時にも注意しないといけない*/
	radianRange = MaxRadian - MinRadian;
	/*範囲を保存する*/
	maxRadian = MaxRadian;
	minRadian = MinRadian;
	originPoint = { 0,20,0 };//原点

	for (int i = FIRE_DISTANCE::ANGLE_100_P; i <= FIRE_DISTANCE::ANGLE_0_P; i++) {
		testAmmo.SetSpeed(speed);//速度を設置する
		testAmmo.SetRadianOnY(radianRange*(1 - i * 0.2) + MinRadian);//角度を設置する
		testAmmo.SetCoord(originPoint);//座標を0にする
		CalTheAnswer();//落下地点を真似て計算する
		distance[i] = Distance2D(originPoint,answer);//距離を計算する
	}
}

void FireController::CalTheAnswer() {
	testAmmo.SetSpeed(speed);
	while (testAmmo.ReferCoordY() > 0) {
		testAmmo.Move();//海面に落ちるまで移動する
	}
	answer = testAmmo.ReferCoord();//落下座標を記録する
}

/*この関数について：
まだすべての状況の検証は完成しなかったため、問題が起こる可能性が予想できる*/
double FireController::CalDistanceAndTellMeRadianOnY(double Distance) {
	for (int i = FIRE_DISTANCE::ANGLE_100_P; i < FIRE_DISTANCE::ANGLE_0_P; i++) {
		/*もしこの距離はこの範囲内であれば*/
		if (Distance >= distance[i + 1] && Distance <= distance[i]) {
			return FindTheDistance(Distance, radianRange*(1 - i * 0.2) + minRadian,
				radianRange*(1 - (i + 1)*0.2) + minRadian);//計算した角度を返す
		}
	}
	/*範囲超えたら、最大もしくは最小距離を返す*/
	if (Distance > distance[FIRE_DISTANCE::ANGLE_100_P])
		return maxRadian;
	if (Distance < distance[FIRE_DISTANCE::ANGLE_0_P])
		return minRadian;
}

/*二分法で具体角度を計算する*/
double FireController::FindTheDistance(double Distance, 
	double MaxRadian, double MinRadian) {

	double realRadian = (MaxRadian - MinRadian) / 2 + MinRadian;//二分法で計算する
	
	/*テストデータを設置する*/
	testAmmo.SetSpeed(speed);
	testAmmo.SetRadianOnY(realRadian);
	testAmmo.SetCoord(originPoint);

	CalTheAnswer();//一回模擬を行う
	
	double tempDistance;
	tempDistance = Distance2D(originPoint, answer);

	double result = tempDistance - Distance;//差を計算する

	/*予想では*/
	if (abs(result) < MathAndPhysics::RANGE_ERROR)//誤差範囲内に入ったら返す
		return realRadian;

	if (result > 0) {
		return FindTheDistance(Distance, realRadian, MinRadian);
		//より小さいの部分に探す
	}
	else{
		return FindTheDistance(Distance, MaxRadian, realRadian);
		//より大きいの部分に探す
	}
}