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

	int TargetNum;//���I��ł�G�̔ԍ�
	int EnemyStatus[GameDefine::MAX_UNIT_A_TEAM];
};

