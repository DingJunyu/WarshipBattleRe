#pragma once
#include"DefinedData.h"
#include<cmath>
class Lock
{
public:
	Lock();
	~Lock();
	/*問い合わせ関数*/
	bool ReferLockOn() { return lockOn; }
	int ReferTarget() { return targetNumber; }

	/*設置関数*/
	/*目標の番号を設置する*/
	void SetNumber(int n) { targetNumber = n; }

	void LockOn_Switch() { lockOn = !lockOn; }



private:
	int discoverStatus;

	int targetNumber;//何番の敵をロックしている
	bool lockOn;
};

