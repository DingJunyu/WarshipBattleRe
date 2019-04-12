#pragma once
#include"DefinedData.h"
#include"DxLib.h"
class Lamp
{
public:
	Lamp() : on(false) {}
	~Lamp();

	//Coordinate2D<float>,���a
	void Inif(Coordinate2D<float> Coord, float Radius) {
		coord = Coord;
		radius = Radius;
	}//�������֐�
	void Draw();//�`��֐�
	void ChangeStatus() { on = !on; }//��ԕύX
	void SetOn() { on = true; }
	void SetOff() { on = false; }
	void SetOnOrOff(bool On) { on = On; }

	void SetColor(bool On, int colorNum);

private:
	bool on;
	unsigned int colorWhenOn;//�I���ɂ��鎞�̐F
	unsigned int colorWhenOff;//�I�t�ɂ��鎞�̐F

	Coordinate2D<float> coord;
	float radius;
};

