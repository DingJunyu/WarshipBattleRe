#pragma once
#include"ShipMain.h"
#include"DefinedData.h"
class ShipCard
{
public:
	ShipCard(int* BarHandle) :barHandle(BarHandle) {
		GetGraphSize(*barHandle, &length, &width);
	}
	~ShipCard();

	void DrawCard(Coordinate2D<double> coord);//引数は普通の座標
	void DrawBar(Coordinate2D<double> coord);//引数はマウスの座標

	ShipMain ship;
private:
	
	int NumberChoosed;//この船の本数
	
	bool pointed;//マウスがここに指しているか
	
	int *barHandle;
	int length, width;
};