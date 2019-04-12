#pragma once
#include"DefinedData.h"
#include"DxLib.h"
class Lamp
{
public:
	Lamp() : on(false) {}
	~Lamp();

	//Coordinate2D<float>,半径
	void Inif(Coordinate2D<float> Coord, float Radius) {
		coord = Coord;
		radius = Radius;
	}//初期化関数
	void Draw();//描画関数
	void ChangeStatus() { on = !on; }//状態変更
	void SetOn() { on = true; }
	void SetOff() { on = false; }
	void SetOnOrOff(bool On) { on = On; }

	void SetColor(bool On, int colorNum);

private:
	bool on;
	unsigned int colorWhenOn;//オンにする時の色
	unsigned int colorWhenOff;//オフにする時の色

	Coordinate2D<float> coord;
	float radius;
};

