#include "FrameControl.h"

FrameControl::~FrameControl()
{
}

bool FrameControl::Wait() {
	next_game_tick += SKIP_TICKS;//���̕`�悷�ׂ����Ԃ��v�Z����
	sleep_time = next_game_tick - GetTickCount();//�҂��Ԃ��v�Z����
	if (sleep_time >= 0)
	{
		Sleep(sleep_time);//�҂�
		return true;//���̃t���[����`�悷��
	}
	else
		return false;//���̃t���[����`�悵�Ȃ�
}