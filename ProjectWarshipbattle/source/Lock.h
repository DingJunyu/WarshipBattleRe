#pragma once
#include"DefinedData.h"
#include<cmath>
class Lock
{
public:
	Lock();
	~Lock();
	/*�₢���킹�֐�*/
	bool ReferLockOn() { return lockOn; }
	int ReferTarget() { return targetNumber; }

	/*�ݒu�֐�*/
	/*�ڕW�̔ԍ���ݒu����*/
	void SetNumber(int n) { targetNumber = n; }

	void LockOn_Switch() { lockOn = !lockOn; }



private:
	int discoverStatus;

	int targetNumber;//���Ԃ̓G�����b�N���Ă���
	bool lockOn;
};

