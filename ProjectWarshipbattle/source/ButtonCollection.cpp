#include "ButtonCollection.h"

ButtonCollection::~ButtonCollection(){
}

//メインメニュー用初期化
void ButtonCollection::Inif(PictureLoader *PL) {
	buttonContainer = new Button[ButtonEvent::BUTTON_AMOUNT];

	//メインメニューボタンの初期化
	for (int i = ButtonEvent::NEW_GAME; i <= ButtonEvent::GAME_OVER; i++)
		buttonContainer[i].Inif(
			PL->ReferButtonHandle(i),
			PL->ReferButtonPressedHandle(i)
		);
}

//ゲーム内の初期化
void ButtonCollection::InifForUserInterface(PictureLoader *PL) {
	buttonContainer = new Button[CommandSerial::COUNT];

	//ボタン初期化
	for (int i = CommandSerial::INCREASE_OUTPUT; i < CommandSerial::SELECT; i++)
		buttonContainer[i].Inif(
			PL->ReferButtonIntheGame(i),
			PL->ReferButtonPressedIntheGame(i)
		);
	for (int i = CommandSerial::SELECT;
		i <= CommandSerial::SELECT + CommandSerial::SELECT_RANGE * 2; i++) {
		buttonContainer[i].Inif(
			PL->ReferButtonIntheGame(CommandSerial::SELECT),
			PL->ReferButtonIntheGame(CommandSerial::SELECT));
	}
}

//メモリ解放
void ButtonCollection::FREE() {
	delete[] buttonContainer;
}