#pragma once
#include"DxLib.h"
/*このクラスはインターネットに載せたものに基づいて作りました
  変数の命名などは自分で作ったものと違う*/
/*実際に描画フレームと処理フレームを分けたいです*/
class FrameControl
{
public:
	FrameControl() :
		FRAMES_PER_SECOND(60),
		SKIP_TICKS(1000 / FRAMES_PER_SECOND),
		next_game_tick(GetTickCount()),
		sleep_time(10) {}
	~FrameControl();

	bool Wait();
	void Reset() { next_game_tick = GetTickCount(); }
	void SetFrame(bool up) { 
		if (up) FRAMES_PER_SECOND += 60;
		else FRAMES_PER_SECOND -= 60;
		if (FRAMES_PER_SECOND < 60)
			FRAMES_PER_SECOND = 60;
		if (FRAMES_PER_SECOND > 240)
			FRAMES_PER_SECOND = 240;
		SKIP_TICKS = 1000 / FRAMES_PER_SECOND;
	}

private:
	int FRAMES_PER_SECOND;//毎秒フレーム数
	int SKIP_TICKS;
	unsigned long next_game_tick;
	int sleep_time;
};