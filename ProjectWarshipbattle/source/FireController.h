#pragma once
#include"DefinedData.h"
#include"Ammo.h"
#include"AllMovableObjects.h"
/*このクラスは基底オブジェクトの移動特性を利用して、砲弾などの落下点を予測します*/

class FireController
{
public:
	FireController();
	~FireController();

	void SetSpeed(double Speed) { speed = Speed; }//速度を設置
	void SetRadian(double RadianOnZ, double RadianOnY) {
		radian.y = RadianOnY;
		radian.z = RadianOnZ;
	}//角度を設置
	void SetCoord(Coordinate<double> Coord) {
		testAmmo.SetCoord(Coord);
	}//座標を設置

	Coordinate<double> ReferAnswer() { return answer; }


private:
	void CalTheAnswer();

	double speed;//速度
	RadianNeededIn3D radian;//角度
	AllMovableObjects testAmmo;//シミュレーション用オブジェクト

	Coordinate<double> answer;//落下地点の座標
};

