#pragma once
#include"DxLib.h"
#include"stdio.h"
#include"DefinedData.h"
#include"OtherFunctions.h"
class StatisticBoardData
{
public:
	StatisticBoardData();
	~StatisticBoardData();

	void Inif(int *handle);
	void Read(bool Res);
	bool Update();
	void Free();

	void GetDis(double dis) { S_data.movedDis = dis; }
	void CountShoot() { S_data.shootCount++; }
	void CountHit() { S_data.hitCount++; }
	void CountDamage(int n) { S_data.damage += n; }
	void CountDamageRec(int n) { S_data.damageRecieved += n; }
	void CountKilled() { S_data.killed++; }

private:
	void Draw();

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

	bool win;
	bool press;
};
