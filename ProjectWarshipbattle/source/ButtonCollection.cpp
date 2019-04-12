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
	for (int i = CommandSerial::INCREASE_OUTPUT; i <= CommandSerial::EXIT; i++)
		buttonContainer[i].Inif(
			PL->ReferButtonIntheGame(i),
			PL->ReferButtonPressedIntheGame(i)
		);
}

//���������
void ButtonCollection::FREE() {
	delete[] buttonContainer;
}