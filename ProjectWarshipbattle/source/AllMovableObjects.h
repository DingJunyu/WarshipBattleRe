#pragma once
#include<cmath>
#include"DefinedData.h"
#include"Camera.h"
#include"DxLib.h"

class AllMovableObjects
{
public:
	//飛べるか、飛行機か、点に見えるか、あたり判定使うか
	AllMovableObjects(bool Flyable, bool Plane, bool Point,bool Crashable) :
		flyable(Flyable),
		plane(Plane), alive(true), point(Point), crashable(Crashable) {
		alive = true;
		radianChangePerFrame = 0;
		airResistanceActive = false;
		airResistance = 0.001;
		speedOnZ = 0;
	}
	virtual ~AllMovableObjects();

	//データ問い合わせ関数
	double ReferCoordX() { return coord.x; }
	double ReferCoordY() { return coord.y; }
	double ReferCoordZ() { return coord.z; }
	Coordinate<double> ReferCoord() { return coord; }
	double ReferSpeedOnZ() { return speedOnZ; }
	double ReferSpeedOnY() { return speedOnY; }
	double ReferAirResistance() { return airResistance; }
	double ReferRadianOnZ() { return radianOnZ; }
	double ReferRadianOnY() { return radianOnY; }
	double ReferLength() { return length; }
	double ReferWidth() { return width; }
	double ReferMultiple() { return multiple; }

	//当たり判定用問い合わせ
	double ReferLeft() { return coord.x - width; }
	double ReferRight() { return coord.x; }
	double ReferUpper() { return coord.z - length; }
	double ReferLower() { return coord.z; }

	//状態問い合わせ関数
	bool ReferAlive() { return alive; }
	int *ReferGraphHandle() { return pictureHandle; }

	//移動関数
	void Move();
	void Unmove();
	void FallingDown();
	void SpeedDownbyAirResistance();

	//描画関数
	void SetPictureHandle(int *p) { pictureHandle = p; }
	void SetShadowHandle(int *p) { shadowHandle = p; }
	void Draw(Camera CM);
	void DrawShadow(Camera CM);
	void DrawSub(Camera CM);
	void DrawSubShadow(Camera CM);

	//状態設置関数
	//ステータス設定関数
	//トン単位
	void SetMass(double Mass) { mass = Mass; }
	//ニュートン単位ですが、現実とちょっと違う
	void SetOutput(double Output) { input = Output; }
	void SetMaxOutput(double MaxOutput) { maxInput = MaxOutput; }
	//新しい座標を設定する
	void NewCoordX(double X) { coord.x = X; }
	void NewCoordZ(double Z) { coord.z = Z; }
	void NewCoordY(double Y) { coord.y = Y; }
	void SetCoord(Coordinate<double> NC) { coord = NC; }
	//新しい角度を設定する
	void SetRadianOnZ(double ROZ) { radianOnZ = ROZ; }
	void SetRadianChangePerFrame(double RCPF) { radianChangePerFrame = RCPF; }
	void SetRadianOnY(double ROY) { radianOnY = ROY; }
	//サイズを設定する
	void SetLength(double L) { length = L; }
	void SetWidth(double W) { width = W; }
	void SetMultiple(double M) { multiple = M; }
	void Killed() { alive = false; }
	void SetSpeed(double Spe);
	void SetMaxSpeedOnZ(double Speed) { maxSpeedOnZ = Speed; }
	void SetAirResistance(double AR) { airResistance = AR; }
	void UnsetPlane() { plane = false; }
	

private:
	/*XZは水平座標、Yは高さ*/
	Coordinate<double> coord;
	Coordinate<double> oldCoord;
	/*スピードは水平と垂直に分ける*/
	double speedOnZ;
	double speedOnY;
	double maxSpeedOnZ;//船と飛行機の最大速度
	bool speedOnZLargerThan0;//正数と負数の判定
	bool airResistanceActive;//空気抵抗を使うか
	double airResistance;//空気抵抗
	double radianOnZ;//行く方向(水平)
	double radianChangePerFrame;
	double radianOnY;//行く方向(垂直)
	bool alive;//生きる状態

	/*出力値と重量*/
	double input;
	double maxInput;
	double mass;

	/*継承先で値を与える*/
	bool flyable;//これは飛べるものか（重力影響の有無）
	bool plane;//これは飛べるものだが、重力を克服して飛ぶことができる

	/*長方形を利用して簡単な当たり判定を行う*/
	/*継承先で値を与える*/
	bool point;//点と判断する時は長さなどを使わない
	double length;
	double width;

	//当たり判定が必要かどうか
	bool crashable;
	
	//画像のハンドル
	int *pictureHandle;
	int *shadowHandle;
	double multiple;
};

