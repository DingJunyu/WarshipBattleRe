#include "ButtonCollection.h"

ButtonCollection::~ButtonCollection(){
}

//���C�����j���[�p������
void ButtonCollection::Inif(PictureLoader *PL) {
	buttonContainer = new Button[ButtonEvent::BUTTON_AMOUNT];

	//���C�����j���[�{�^���̏�����
	for (int i = ButtonEvent::NEW_GAME; i <= ButtonEvent::GAME_OVER; i++)
		buttonContainer[i].Inif(
			PL->ReferButtonHandle(i),//���ʂ̃n���h�������
			PL->ReferButtonPressedHandle(i)//�����ꂽ���̃n���h�������
		);
}

//�Q�[�����̏�����
void ButtonCollection::InifForUserInterface(PictureLoader *PL) {
	buttonContainer = new Button[CommandSerial::COUNT];

	//�{�^��������
	for (int i = CommandSerial::INCREASE_OUTPUT; i < CommandSerial::SELECT; i++)
		buttonContainer[i].Inif(
			PL->ReferButtonIntheGame(i),//���ʂ̃n���h�������
			PL->ReferButtonPressedIntheGame(i)//�����ꂽ���̃n���h�������
		);
	/*�G���X�g�Ǝ��@���X�g*/
	for (int i = CommandSerial::SELECT;
		i <= CommandSerial::SELECT + CommandSerial::SELECT_RANGE * 2; i++) {
		buttonContainer[i].Inif(
			PL->ReferButtonIntheGame(CommandSerial::SELECT),//�����ꂽ�Ƃ��Ƃӂ��̎����ς��Ȃ����߁A�������ӂ��ɂ���
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

//���������
void ButtonCollection::FREE() {
	delete[] buttonContainer;
}