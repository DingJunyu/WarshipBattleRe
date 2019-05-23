#include "FrameControl.h"

FrameControl::~FrameControl()
{
}

bool FrameControl::Wait() {
	next_game_tick += SKIP_TICKS;//Ÿ‚Ì•`‰æ‚·‚×‚«ŠÔ‚ğŒvZ‚·‚é
	sleep_time = next_game_tick - GetTickCount();//‘Ò‚ÂŠÔ‚ğŒvZ‚·‚é
	one_frame_time = GetTickCount() - prev_game_tick;
	prev_game_tick = GetTickCount();
	if (sleep_time >= 0)
	{
		Sleep(sleep_time);//‘Ò‚Â
		return true;//‚±‚ÌƒtƒŒ[ƒ€‚ğ•`‰æ‚·‚é
	}
	else
		return false;//‚±‚ÌƒtƒŒ[ƒ€‚ğ•`‰æ‚µ‚È‚¢
}

double FrameControl::ReferFPS() {
	return 1000 / (double)one_frame_time;
}