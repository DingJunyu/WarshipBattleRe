#include "FrameControl.h"

FrameControl::~FrameControl()
{
}

void FrameControl::Wait() {
	next_game_tick += SKIP_TICKS;//ŽŸ‚Ì•`‰æ‚·‚×‚«ŽžŠÔ‚ðŒvŽZ‚·‚é
	sleep_time = next_game_tick - GetTickCount();//‘Ò‚ÂŽžŠÔ‚ðŒvŽZ‚·‚é
	if (sleep_time >= 0)
	{
		Sleep(sleep_time);//‘Ò‚Â
	}
}