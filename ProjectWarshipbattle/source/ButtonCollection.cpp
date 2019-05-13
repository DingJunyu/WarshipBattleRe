#include "ButtonCollection.h"

ButtonCollection::~ButtonCollection(){
}

//メインメニュー用初期化
void ButtonCollection::Inif(PictureLoader *PL) {
	buttonContainer = new Button[ButtonEvent::BUTTON_AMOUNT];

	//メインメニューボタンの初期化
	for (int i = ButtonEvent::NEW_GAME; i <= ButtonEvent::GAME_OVER; i++)
		buttonContainer[i].Inif(
			PL->ReferButtonHandle(i),//普通のハンドルを取る
			PL->ReferButtonPressedHandle(i)//押された時のハンドルを取る
		);
}

//ゲーム内の初期化
void ButtonCollection::InifForUserInterface(PictureLoader *PL) {
	buttonContainer = new Button[CommandSerial::COUNT];

	//ボタン初期化
	for (int i = CommandSerial::INCREASE_OUTPUT; i < CommandSerial::SELECT; i++)
		buttonContainer[i].Inif(
			PL->ReferButtonIntheGame(i),//普通のハンドルを取る
			PL->ReferButtonPressedIntheGame(i)//押された時のハンドルを取る
		);
	/*敵リストと自機リスト*/
	for (int i = CommandSerial::SELECT;
		i <= CommandSerial::SELECT + CommandSerial::SELECT_RANGE * 2; i++) {
		buttonContainer[i].Inif(
			PL->ReferButtonIntheGame(CommandSerial::SELECT),//押されたときとふつうの時が変わらないため、両方もふつうにする
			PL->ReferButtonIntheGame(CommandSerial::SELECT));
	}
	for (int i = CommandSerial::SELECT_IN_FORMATION;
		i <= CommandSerial::SELECT_IN_FORMATION + 16; i++) {
		if (i % 2 == 0)
			buttonContainer[i].Inif(
				PL->ReferButtonIntheGame(CommandSerial::PLUS),
				PL->ReferButtonPressedIntheGame(CommandSerial::PLUS));
		else
			buttonContainer[i].Inif(
				PL->ReferButtonIntheGame(CommandSerial::MINUS),
				PL->ReferButtonPressedIntheGame(CommandSerial::MINUS));
	}
}

//メモリ解放
void ButtonCollection::FREE() {
	delete[] buttonContainer;
}