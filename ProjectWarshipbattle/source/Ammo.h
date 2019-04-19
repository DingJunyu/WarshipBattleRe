#pragma once
#include "AllMovableObjects.h"
class Ammo :
	public AllMovableObjects
{
public:
	Ammo() : AllMovableObjects(true, //飛べるもの
		false, //飛行機ではない
		true, //点として計算する
		true) {//あたり判定が有効
		isUsable = true;
		damage = 5;
	}
	/*初期化時に必要なもの:
	速度、角度、ＸＺＹ座標、Ｙ軸の角度、弾のハンドル、船の番号*/
	Ammo(double Speed, double Radian, double CoordX, double CoordZ, double CoordY,
		double RadianOnY, int *AmmoHandle,int SerialNumber) :
		AllMovableObjects(true, false, true, true) {
		SetRadianOnZ(Radian);
		NewCoordX(CoordX);
		NewCoordZ(CoordZ);
		NewCoordY(CoordY);
		SetRadianOnY(RadianOnY);
		SetSpeed(Speed);
		ammoHandle = AmmoHandle;
		serialNumber = SerialNumber;
		SetData();//画像サイズを取る

		isUsable = true;
		damage = 10;
	}
	~Ammo();

	double ReferDamage() { return damage; }
	void CheckAlive();//海に落ちたのか
	bool ReferUsable() { return isUsable; }
	void SetData();//画像のサイズを読み込む
	void Unusable() { isUsable = false; }
	int ReferSerialNumber() { return serialNumber; }
	
	//通常描画
	void Draw(int x,int z);
	//消す前の動画演出
	void DrawDelete(int x, int z);
	bool FallIntoWater() { 
		if (ReferCoordY() <= 0)return true; 
		else return false;
	}
	
private:
	double penetrateDepth;//貫通できるアーマーの最大厚さ
	double damage;//ダメージ

	bool isUsable;//まだつけえるか

	/*画像サイズ*/
	int graphX;
	int graphZ;

	/*画像ハンドル*/
	int *ammoHandle;

	/*識別番号*/
	int serialNumber;
};

