#pragma once
#include"ShipMain.h"
#include"DefinedData.h"
class ShipCard
{
public:
	ShipCard(int* BarHandle,int *FN) :barHandle(BarHandle),
		fantasyNumber(FN){
		flagHandle = barHandle + 1;
		GetGraphSize(*barHandle, &length, &width);
		GetGraphSize(*fantasyNumber, &numL, &numW);
		GetGraphSize(*flagHandle, &flagL, &flagW);
		NumberChoosed = 0;
		flag = false;
	}
	~ShipCard();

	void DrawCard(Coordinate2D<double> coord);//引数は普通の座標
	void DrawNumber(Coordinate2D<double> coord);
	void DrawBar(Coordinate2D<double> coord);//引数はマウスの座標

	void ChangeFlagStatus() { flag = false; }
	void SetFlag() { flag = true; }
	void Plus() { NumberChoosed++; if (NumberChoosed > 10) NumberChoosed = 10; }
	void Minus() { NumberChoosed--; if (NumberChoosed < 0)NumberChoosed = 0; }
	bool Clicked(Coordinate2D<int> Coord, Coordinate2D<int> Mouse);
	bool ReferFlag() { return flag; }

	int ReferNumber() { return NumberChoosed; }

	ShipMain ship;
private:
	
	int NumberChoosed;//この船の本数
	
	bool pointed;//マウスがここに指しているか
	bool flag;
	
	int *barHandle;
	int *fantasyNumber;
	int *flagHandle;
	int length, width;
	int numL, numW;
	int flagL, flagW;
};