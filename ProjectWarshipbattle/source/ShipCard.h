#pragma once
#include"ShipMain.h"
#include"DefinedData.h"
class ShipCard
{
public:
	ShipCard(int* BarHandle,int *FN) :barHandle(BarHandle),
		fantasyNumber(FN){
		GetGraphSize(*barHandle, &length, &width);
		GetGraphSize(*fantasyNumber, &numL, &numW);
		NumberChoosed = 0;
	}
	~ShipCard();

	void DrawCard(Coordinate2D<double> coord);//引数は普通の座標
	void DrawNumber(Coordinate2D<double> coord);
	void DrawBar(Coordinate2D<double> coord);//引数はマウスの座標

	void Plus() { NumberChoosed++; if (NumberChoosed > 10) NumberChoosed = 10; }
	void Minus() { NumberChoosed--; if (NumberChoosed < 0)NumberChoosed = 0; }

	int ReferNumber() { return NumberChoosed; }

	ShipMain ship;
private:
	
	int NumberChoosed;//この船の本数
	
	bool pointed;//マウスがここに指しているか
	
	int *barHandle;
	int *fantasyNumber;
	int length, width;
	int numL, numW;
};