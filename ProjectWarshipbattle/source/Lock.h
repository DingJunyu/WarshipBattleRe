#pragma once
#include"DefinedData.h"
#include<cmath>
class Lock
{
public:
	Lock();
	~Lock();
	/*�₢���킹�֐�*/
	Coordinate<double> ReferNextPosition() { return nextPosition; }
	RadianNeededIn3D Refercorrection() { return correction; }
	bool ReferLockOn() { return lockOn; }
	int ReferTarget() { return targetNumber; }

	/*�ݒu�֐�*/
	/*�ڕW�̔ԍ���ݒu����*/
	void SetNumber(int n) { targetNumber = n; }

	void LockOn_Switch() { lockOn = !lockOn; }

	void Set_IntialLead_Seconds(bool up);
	void ChangeCorrection_Distance(bool up);
	void Reset_IntialLead_Seconds() { initialLead_Seconds = 0; }

	/*�����֐�*/
	void CalNextPosition(Coordinate<double> coord,double speed,double radian);

private:
	int discoverStatus;

	int targetNumber;//���Ԃ̓G�����b�N���Ă���
	bool lockOn;

	double initialLead_Seconds;//���������C����
	RadianNeededIn3D correction;//�v���[���[�ݒu�����C����

	Coordinate<double> nextPosition;//x�b��G�̈ʒu
};

