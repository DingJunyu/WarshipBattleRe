#pragma once
class AntiAir
{
public:
	AntiAir();
	~AntiAir();
	
private:
	//パーツのHP
	int heartPoint;
	int MaxHeartPoint;
	//パーツの相対位置とサイズ
	double relativeCoordX;
	double relativeCoordZ;
	//有効範囲
	double horizontalRange;
	double verticalRange;

	int ammoRemain;
	double hitRate;
};

