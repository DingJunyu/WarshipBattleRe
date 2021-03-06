#include "FrameControl.h"

FrameControl::~FrameControl()
{
}

bool FrameControl::Wait() {
	next_game_tick += SKIP_TICKS;//次の描画すべき時間を計算する
	sleep_time = next_game_tick - GetTickCount();//待つ時間を計算する
	one_frame_time = GetTickCount() - prev_game_tick;
	prev_game_tick = GetTickCount();
	if (sleep_time >= 0)
	{
		Sleep(sleep_time);//待つ
		return true;//このフレームを描画する
	}
	else
		return false;//このフレームを描画しない
}

double FrameControl::ReferFPS() {
	return 1000 / (double)one_frame_time;
}