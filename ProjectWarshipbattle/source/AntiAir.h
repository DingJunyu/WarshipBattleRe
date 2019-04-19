#pragma once
/*船に乗せた対空武器を実装するためのクラス*/
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

	int ammoRemain;//残弾数
	double hitRate;//射撃速度
};

