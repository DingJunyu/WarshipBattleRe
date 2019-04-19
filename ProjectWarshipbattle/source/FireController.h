#pragma once
#include"DefinedData.h"
#include"Ammo.h"
#include"AllMovableObjects.h"
#include"OtherFunctions.h"
/*このクラスは基底オブジェクトの移動特性を利用して、砲弾などの落下点を予測します*/

class FireController
{
public:
	FireController();
	~FireController();

	/*設置など*/
	void SetSpeed(double Speed) { 
		speed = Speed;
	}//速度を設置
	void SetRadian(RadianNeededIn3D temp) {
		radian = temp;
		testAmmo.SetRadianOnY(temp.y);
		testAmmo.SetRadianOnZ(temp.z);
	}//角度を設置
	void SetCoord(Coordinate<double> Coord) {
		testAmmo.SetCoord(Coord);
	}//座標を設置

	/*問い合わせ関数*/
	Coordinate<double> ReferAnswer() { return answer; }
	double ReferMaxRange() { return distance[FIRE_DISTANCE::ANGLE_100_P]; }
	double ReferMinRange() { return distance[FIRE_DISTANCE::ANGLE_0_P]; }

	/*計算関数*/
	void InifDistance(double MaxRadian,double MinRadian);
	void CalTheAnswer();

	double CalDistanceAndTellMeRadianOnY(double Distance);

private:
	double FindTheDistance(double Distance, double MaxRadian, double MinRadian);

	double speed;
	
	RadianNeededIn3D radian;//角度
	AllMovableObjects testAmmo;//シミュレーション用オブジェクト

	double distance[FIRE_DISTANCE::ANGLE_TYPE_AMOUNT];
	
	double maxRadian;//垂直最大角度
	double minRadian;//垂直最小角度
	double radianRange;

	Coordinate<double> originPoint;
	Coordinate<double> answer;//落下地点の座標
};
