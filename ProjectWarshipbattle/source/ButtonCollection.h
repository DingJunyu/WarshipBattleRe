#pragma once
#include"Button.h"
#include"PictureLoader.h"
#include"SoundLoader.h"
class ButtonCollection{
public:
	ButtonCollection() {}
	~ButtonCollection();

	void Inif(PictureLoader *PL);//���C�����j���[�̏�����
	void InifForUserInterface(PictureLoader *PL);//�Q�[�����̏�����
	void FREE();

	Button * buttonContainer;

private:
	int buttonAmount;
};

