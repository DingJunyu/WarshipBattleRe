#pragma once
#include"DefinedData.h"
#include"SoundLoader.h"
#include"DxLib.h"
/*�����̃R���g���[���[�ł́A�L�[�{�[�h�̓��͂��󂯎���ĐM���ɕύX���܂��B
�}�E�X����Ȃǂ�ClickableUserInterface�Ɏ������܂�*/
class Controller
{
public:
	Controller();
	~Controller();

	void Inif(SoundLoader *SL);

	int GetCommand();//�L�[�{�[�h���͂�����āA�R�}���h�ɕύX�������ƕԂ�

private:
	int Action;
	char KeyBuf[256];

	/*�e�X�g�p�����}�[�N*/
	int shipHordType;
	int *shipHordHigh;
	int *shipHordLow;
};

