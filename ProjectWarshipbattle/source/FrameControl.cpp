#include "FrameControl.h"

FrameControl::~FrameControl()
{
}

void FrameControl::Wait() {
	next_game_tick += SKIP_TICKS;//���̕`�悷�ׂ����Ԃ��v�Z����
	sleep_time = next_game_tick - GetTickCount();//�҂��Ԃ��v�Z����
	if (sleep_time >= 0)
	{
		Sleep(sleep_time);//�҂�
	}
}