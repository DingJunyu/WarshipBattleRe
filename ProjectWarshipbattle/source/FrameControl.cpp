#include "FrameControl.h"

FrameControl::~FrameControl()
{
}

bool FrameControl::Wait() {
	next_game_tick += SKIP_TICKS;//���̕`�悷�ׂ����Ԃ��v�Z����
	sleep_time = next_game_tick - GetTickCount();//�҂��Ԃ��v�Z����
	one_frame_time = GetTickCount() - prev_game_tick;
	prev_game_tick = GetTickCount();
	if (sleep_time >= 0)
	{
		Sleep(sleep_time);//�҂�
		return true;//���̃t���[����`�悷��
	}
	else
		return false;//���̃t���[����`�悵�Ȃ�
}

double FrameControl::ReferFPS() {
	return 1000 / (double)one_frame_time;
}