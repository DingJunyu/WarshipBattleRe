#pragma once
#include"DefinedData.h"
#include"ShipMain.h"
class PlayerStatus
{
public:
	PlayerStatus();
	~PlayerStatus();

private:
	double distance;

	int TargetNum;//今選んでる敵の番号
	int EnemyStatus[GameDefine::MAX_UNIT_A_TEAM];
};

