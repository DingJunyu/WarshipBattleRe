#pragma once
#include"Button.h"
#include"PictureLoader.h"
#include"SoundLoader.h"
class ButtonCollection{
public:
	ButtonCollection() {}
	~ButtonCollection();

	void Inif(PictureLoader *PL);//メインメニューの初期化
	void InifForUserInterface(PictureLoader *PL);//ゲーム中の初期化
	void FREE();

	Button * buttonContainer;

private:
	int buttonAmount;
};

