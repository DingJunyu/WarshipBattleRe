#pragma once
class ShipCrashParts
{
public:
	ShipCrashParts() {
		damaged = false;
		isMainPart = false;
	}
	~ShipCrashParts();

	//データ問い合わせ関数
	double ReferRelativeCoordX() { return relativeCoordX; }
	double ReferRelativeCoordZ() { return relativeCoordZ; }
	double ReferLength() { return length; }
	double ReferWidth() { return width; }
	int ReferHeartPoint() { return heartPoint; }

	//状態問い合わせ関数
	bool ReferDamaged() { return damaged; }
	bool ReferMainPart() { return isMainPart; }

	void SetMainPart() { isMainPart = true; }

	//操作関数
	void MinusHP(int damage);

	//初期化関数など
	void SetRelativeCoordX(double RCX) { relativeCoordX = RCX; }
	void SetRelativeCoordZ(double RCZ) { relativeCoordZ = RCZ; }
	void SetLength(double L) { length = L; }
	void SetWidth(double W) { width = W; }

private:
	//パーツのHP
	int heartPoint;
	int MaxHeartPoint;
	//パーツの相対位置とサイズ
	double relativeCoordX;
	double relativeCoordZ;
	double length;
	double draft;
	double width;
	double height;//高さ-当たり判定用
	//パーツはも壊れたか
	bool damaged;
	bool isMainPart;//このパーツは大事なパーツだか
	double armorOntheSide;
	double armorOntheTop;
};