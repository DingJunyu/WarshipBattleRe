#include "ButtonCollection.h"

ButtonCollection::~ButtonCollection(){
}

//���C�����j���[�p������
void ButtonCollection::Inif(PictureLoader *PL) {
	buttonContainer = new Button[ButtonEvent::BUTTON_AMOUNT];

	//���C�����j���[�{�^���̏�����
	for (int i = ButtonEvent::NEW_GAME; i <= ButtonEvent::GAME_OVER; i++)
		buttonContainer[i].Inif(
			PL->ReferButtonHandle(i),
			PL->ReferButtonPressedHandle(i)
		);
}

//�Q�[�����̏�����
void ButtonCollection::InifForUserInterface(PictureLoader *PL) {
	buttonContainer = new Button[CommandSerial::COUNT];

	//�{�^��������
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

//���������
void ButtonCollection::FREE() {
	delete[] buttonContainer;
}