#pragma once
#include"DxLib.h"
/*���̃N���X�̓C���^�[�l�b�g�ɍڂ������̂Ɋ�Â��č��܂���
  �ϐ��̖����Ȃǂ͎����ō�������̂ƈႤ*/
class FrameControl
{
public:
	FrameControl() :
		FRAMES_PER_SECOND(60),
		SKIP_TICKS(1000 / FRAMES_PER_SECOND),
		next_game_tick(GetTickCount()),
		sleep_time(10) {}
	~FrameControl();

	void Wait();
	void Reset() { next_game_tick = GetTickCount(); }

private:
	const int FRAMES_PER_SECOND;
	const int SKIP_TICKS;
	unsigned long next_game_tick;
	int sleep_time;
};