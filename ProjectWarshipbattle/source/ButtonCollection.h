#pragma once
#include"Button.h"
#include"PictureLoader.h"
#include"SoundLoader.h"
/*このクラスはボタンを保存するためのクラス、
ボタンを使いたい場合はこのクラスを宣言する*/
class ButtonCollection{
public:
	ButtonCollection() {}
	~ButtonCollection();

	void Inif(PictureLoader *PL);//メインメニューの初期化
	void InifForUserInterface(PictureLoader *PL);//ゲーム中の初期化
	void FREE();

	Button * buttonContainer;//ボタン宣言

private:
	int buttonAmount;
};

