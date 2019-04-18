#pragma once
#include"DefinedData.h"
#include<cmath>
class Lock
{
public:
	Lock();
	~Lock();
	/*問い合わせ関数*/
	Coordinate<double> ReferNextPosition() { return nextPosition; }
	RadianNeededIn3D Refercorrection() { return correction; }
	bool ReferLockOn() { return lockOn; }
	int ReferTarget() { return targetNumber; }

	/*設置関数*/
	/*目標の番号を設置する*/
	void SetNumber(int n) { targetNumber = n; }

	void LockOn_Switch() { lockOn = !lockOn; }

	void Set_IntialLead_Seconds(bool up);
	void ChangeCorrection_Distance(bool up);
	void Reset_IntialLead_Seconds() { initialLead_Seconds = 0; }

	/*処理関数*/
	void CalNextPosition(Coordinate<double> coord,double speed,double radian);

private:
	int discoverStatus;

	int targetNumber;//何番の敵をロックしている
	bool lockOn;

	double initialLead_Seconds;//初期未来修正量
	RadianNeededIn3D correction;//プレーヤー設置した修正量

	Coordinate<double> nextPosition;//x秒後敵の位置
};

