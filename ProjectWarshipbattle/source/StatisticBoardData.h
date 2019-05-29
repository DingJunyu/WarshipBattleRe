#pragma once
#include"DxLib.h"
#include"stdio.h"
#include"DefinedData.h"
#include"ColorBox.h"
#include"OtherFunctions.h"
class StatisticBoardData
{
public:
	StatisticBoardData();
	~StatisticBoardData();

	void Inif(int *handle);
	void Read(bool Res);
	bool Update();
	
	void GetDis(double dis) { S_data.movedDis = dis; }
	void CountShoot() { S_data.shootCount++; }
	void CountHit() { S_data.hitCount++; }
	void CountDamage(int n) { S_data.damage += n; }
	void CountDamageRec(int n) { S_data.damageRecieved += n; }
	void CountKilled() { S_data.killed++; }

private:
	void Draw();
	void DrawStar(Coordinate2D<int> coord, Coordinate2D<int> coord2);

	long long int frameCount;
	int *handleList;//
	const int seconds = 60;

	enum StaBoard {
		TOTAL_KILL = 0,
		TOTAL_MOVE,
		TOTAL_DAMAGE,
		TOTAL_DAMAGE_RECIEVED,
		MAX_DAMAGE,
		MAX_MOVE,
		MAX_HITRATE
	};

	double boardData[7];
	StatisticData S_data;

	/*キー押す事件を記録*/
	bool win;
	bool press;
	long long int pressFrame;

	/*ハイスコア*/
	bool newHighScore[7];

	ColorBox CrBox;
	int CrNum;

	const char *RecordChar[7] = {
		"隻","海里","Pt","Pt","Pt","海里","%"
	};
};
