#include "Engine.h"

Engine::~Engine()
{
}

void Engine::Update() {
	if (outPutPercentage > 0) {
		//���傷�鎞�͑傫����Α傫���قǑ������x��������
		if (currentOutput <= maxOutput * outPutPercentage) {
			if (currentOutput < 0)//���΂ɂȂ鎞�͑��߂ɑ��傷��
				currentOutput += 0.1;
			else
				/*��{�������A�����ɋ߂Â��Ƒ�������������*/
				currentOutput += baseIncreaseRate * increaseDownWithOutputPercentage *
				(1 - (currentOutput + 0.01) / maxOutput);
		}
		else {//�o�̓p�[�Z���e�[�W�͍��̏󋵂�荂�����ɏo�͂�������茸������
				currentOutput -= baseIncreaseRate * increaseDownWithOutputPercentage *
				(currentOutput + 0.01) / maxOutput;
		}
	}
	if (outPutPercentage < 0) {
		if (currentOutput >= maxOutput * outPutPercentage) {
			if (currentOutput > 0)//���΂ɂȂ鎞�͑��߂Ɍ�������
				currentOutput -= 0.1;
			else
				/*��{�������A�����ɋ߂Â��Ƒ�������������*/
				currentOutput -= baseIncreaseRate * increaseDownWithOutputPercentage*
				abs(1 - (currentOutput - 0.01)) / maxOutput;
		}
		else {//�o�̓p�[�Z���e�[�W�͍��̏󋵂�荂�����ɏo�͂�������茸������
				currentOutput += baseIncreaseRate * increaseDownWithOutputPercentage *
				abs(currentOutput - 0.01) / maxOutput;
		}
	}
	if (outPutPercentage == 0) {//�o�͂͂O�̎��ɌŒ�P�ʂŌ���
		if (currentOutput > 0.1)
			currentOutput -= 0.1;
		if (currentOutput < -0.1)
			currentOutput += 0.1;
		if (currentOutput >= -.1&&currentOutput <= .1)
			currentOutput = 0;
	}
}
