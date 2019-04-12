#pragma once
#include"IngameDataManagement.h"
class TeamDeathMatchControl
{
public:
	TeamDeathMatchControl();
	~TeamDeathMatchControl();

	bool GameOver(bool TD) { return TD; }
	void DrawBoard(IngameDataManagement IDM);

private:
	int TeamACount;
	int TeamARemained;
	int TeamBCount;
	int TeamBRemained;
};

