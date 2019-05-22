#include "Controller.h"

Controller::Controller()
{
}

Controller::~Controller()
{
}

int Controller::GetCommand() {
	GetHitKeyStateAll(KeyBuf);
	//�o�̓R���g���[��
	if (KeyBuf[KEY_INPUT_UP] && GetInputChar(TRUE))
		return CommandSerial::INCREASE_OUTPUT;
	if (KeyBuf[KEY_INPUT_DOWN] && GetInputChar(TRUE))
		return CommandSerial::DECREASE_OUTPUT;

	//�����R���g���[��
	if (KeyBuf[KEY_INPUT_RIGHT])
		return CommandSerial::TURN_RIGHT;
	if (KeyBuf[KEY_INPUT_LEFT])
		return CommandSerial::TURN_LEFT;
	if (KeyBuf[KEY_INPUT_X] && GetInputChar(TRUE))
		return CommandSerial::TURN_RETURN;

	//�ˌ�
	if (KeyBuf[KEY_INPUT_S] && GetInputChar(TRUE))
		return CommandSerial::SHOOT;

	//�e�X�g�r���[
	if (KeyBuf[KEY_INPUT_0] && GetInputChar(TRUE))
		return CommandSerial::TEST_VIEW_ON;

	//Debug
//	if (KeyBuf[KEY_INPUT_P] && GetInputChar(TRUE))
//		WaitKey();
	if (KeyBuf[KEY_INPUT_NUMPAD9] && GetInputChar(TRUE))
		return CommandSerial::TEST_INCREASE_FRAME;
	if (KeyBuf[KEY_INPUT_NUMPAD6] && GetInputChar(TRUE))
		return CommandSerial::TEST_DECREASE_FRAME;

	/*�����̂��̂�V���Ȋ֐��ɐ�������*/
	if (KeyBuf[KEY_INPUT_Z] && GetInputChar(TRUE)) {
		PlaySoundMem(*shipHordHigh, DX_PLAYTYPE_BACK, TRUE);
	}
	if (KeyBuf[KEY_INPUT_C] && GetInputChar(TRUE)) {
		PlaySoundMem(*shipHordLow, DX_PLAYTYPE_BACK, TRUE);
	}

	if (KeyBuf[KEY_INPUT_9])
		return CommandSerial::GET_DAMAGE_TEST;

	if (KeyBuf[KEY_INPUT_ESCAPE])
		return CommandSerial::EXIT;

	int mouseRot = GetMouseWheelRotVol();

	if (mouseRot > 0)
		return CommandSerial::ZOOM_IN;
	if (mouseRot < 0)
		return CommandSerial::ZOOM_OUT;



	return CommandSerial::NONE_COMMAND;
}

void Controller::Inif(SoundLoader *SL) {
	shipHordHigh = SL->referShipHordLargeHigh();//���̃f�[�^��ݒu����
	shipHordLow = SL->referShipHordLargeLow();
}