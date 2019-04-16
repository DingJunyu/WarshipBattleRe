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

	void SetSpeed(double Speed) { testAmmo.SetSpeed(Speed); }//速度を設置
	void SetRadian(RadianNeededIn3D temp) {
		radian = temp;
		testAmmo.SetRadianOnY(temp.y);
		testAmmo.SetRadianOnZ(temp.z);
	}//角度を設置
	void SetCoord(Coordinate<double> Coord) {
		testAmmo.SetCoord(Coord);
	}//座標を設置

	Coordinate<double> ReferAnswer() { return answer; }
	void CalTheAnswer();

private:
	RadianNeededIn3D radian;//角度
	AllMovableObjects testAmmo;//シミュレーション用オブジェクト

	Coordinate<double> answer;//落下地点の座標
};

