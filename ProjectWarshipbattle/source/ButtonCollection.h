#pragma once
#include"Button.h"
#include"PictureLoader.h"
#include"SoundLoader.h"
/*���̃N���X�̓{�^����ۑ����邽�߂̃N���X�A
�{�^�����g�������ꍇ�͂��̃N���X��錾����*/
class ButtonCollection{
public:
	ButtonCollection() {}
	~ButtonCollection();

	void Inif(PictureLoader *PL);//���C�����j���[�̏�����
	void InifForUserInterface(PictureLoader *PL);//�Q�[�����̏�����
	void FREE();

	Button * buttonContainer;//�{�^���錾

private:
	int buttonAmount;
};

