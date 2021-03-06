#pragma once
#include"DxLib.h"
#include"DefinedData.h"
#include"Camera.h"
#include<time.h>
#include<cmath>
class Effect
{
public:
	Effect();
	Effect(int Type, bool Movable, int continueTime, double Radian,
		double TargetRadian, double RadianChangePerFrame, double Speed, double CoordX,
		double CoordZ, int *GraphicHandle, bool Spread, 
		double ZoomMutliple, double ZoomRate) : type(Type), movable(Movable),
		timeUp(false), endTime(GetNowCount() + continueTime),
		radian(Radian), targetRadian(TargetRadian),
		radianChangePerFrame(RadianChangePerFrame),
		speed(Speed), coord(CoordX,CoordZ), graphicHandle(GraphicHandle),
		spread(Spread), zoomMutliple(ZoomMutliple), zoomRate(ZoomRate)
	{
		right = rand() % 2;
		GetGraphSize(*graphicHandle, &graphX, &graphZ);
		nextFlame = 0;
	}
	~Effect();

	void Draw(Camera CM);
	void DrawMark(Coordinate2D<int> Coord);//命中などを描く時に使う描画方式
	void Move();

	//時間に過ぎたら消す
	bool ReferTimeUp() { return timeUp; }
	void Update() { Check(); }

	void SetGraphHandle(int *GH) { graphicHandle = GH; }

private:
	void Check();//時間が切れたかを確認
	//エフェクトのタイプと移動できるか
	bool movable;
	int type;
	//座標など
	double radian;//ラジアン
	double targetRadian;//目標ラジアン
	double radianChangePerFrame;//毎フレームの変更角度
	Coordinate2D<double> coord;

	double speed;//移動速度
	bool right;//右に移動するか
	//画像データ
	int *graphicHandle;
	int graphX;
	int graphZ;

	//消失時間
	int endTime;
	bool timeUp;

	bool spread;//自分が拡大できるか
	double zoomMutliple;//倍率
	double zoomRate;//フレームごとの拡大率

	int nextFlame;
	int flame;
};