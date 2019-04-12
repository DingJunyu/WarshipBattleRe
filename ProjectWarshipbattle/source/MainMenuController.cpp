#include "MainMenuController.h"

MainMenuController::~MainMenuController()
{
}

void MainMenuController::Inif() {
	PL.InifForMainMenu();
	SL.Inif();
	BC.Inif(&PL);

	backGroundHandle = PL.ReferBackGround();
	title = PL.ReferTitle();
	pressedToStart = PL.ReferPressedToStart();
	pressedToStartDown = PL.ReferPressedToStartDown();
	//�g����̃T�C�Y�����
	GetGraphSize(*title, &titleSizeX, &titleSizeZ);
	GetGraphSize(*pressedToStart, &PTSSizeX, &PTSSizeZ);

	SetButtonPosition();//�e�{�^���̈ʒu��ݒ肷��
}

void MainMenuController::DrawTitle() {
	SetDrawScreen(DX_SCREEN_BACK);//����ʂɕ`�悷��
	
	while (1) {
		ClearDrawScreen();

		/*�w�i��`�悷��*/
		DrawExtendGraph(0, 0, Screen::SCREEN_X, Screen::SCREEN_Z,
			*backGroundHandle, FALSE);
		/*�^�C�g����`�悷��*/
		DrawExtendGraph(
			Screen::SCREEN_X / 2 - titleSizeX / 4,
			(int)(0.1*Screen::SCREEN_Z),
			Screen::SCREEN_X / 2 + titleSizeX / 4,
			(int)(0.1*Screen::SCREEN_Z + titleSizeZ / 2),
			*title,
			TRUE
		);

		//�R�O�t���[�����Ƃɓ_�ł���
		int *useThisPressedToStart =
			(countForTitle % 60 > 30 ? pressedToStart : pressedToStartDown);

		DrawExtendGraph(
			Screen::SCREEN_X / 2 - PTSSizeX / 4,
			(int)(0.6*Screen::SCREEN_Z),
			Screen::SCREEN_X / 2 + PTSSizeX / 4,
			(int)(0.6*Screen::SCREEN_Z) + PTSSizeZ / 2,
			*useThisPressedToStart,
			TRUE
		);

		/*�摜�A�o*/
		FC.Wait();
		ScreenFlip();

		countForTitle++;//30�t���[�����Ƃɓ_�ł���//0.5�b
		countForTitle %= 60;//�I�[�o�[�t���[�΍�

		//�C���v�b�g������ΏI���
		if ((CheckHitKeyAll() != 0 && GetInputChar(TRUE))
			|| GetMouseInput() == MOUSE_INPUT_LEFT)
			break;

		if (ProcessMessage() == -1)
			break;
	}
}

void MainMenuController::DrawMainMenu() {
	if (firstTimeGetIntoMainMenu) {
		FC.Reset();
		firstTimeGetIntoMainMenu = false;
	}

	SetDrawScreen(DX_SCREEN_BACK);//����ʂɕ`�悷��
	ClearDrawScreen();

	/*�w�i��`��*/
	DrawExtendGraph(0, 0, Screen::SCREEN_X, Screen::SCREEN_Z,
		*backGroundHandle, FALSE);

	/*�{�^����`��*/
	for (int i = ButtonEvent::NEW_GAME; i <= ButtonEvent::GAME_OVER; i++) {
		BC.buttonContainer[i].DrawThisButton();
	}

	/*�^�C�g����`��*/
	DrawExtendGraph(
		(int)(.67*Screen::SCREEN_X), 
		(int)(.1*Screen::SCREEN_Z), 
		(int)(.67*Screen::SCREEN_X + titleSizeX / 4), 
		(int)(.1*Screen::SCREEN_Z + titleSizeZ / 4),
		*title, TRUE);

	ScreenFlip();
}

void MainMenuController::DrawLoading() {
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	DrawExtendGraph(0, 0, Screen::SCREEN_X, Screen::SCREEN_Z,
		*backGroundHandle, FALSE);

	unsigned int Cr;
	Cr = GetColor(255, 255, 255);
	SetFontSize(30);

	DrawString((int)(0.85*Screen::SCREEN_X), 
		(int)(0.85*Screen::SCREEN_Z), "LOADING...", Cr);

	ScreenFlip();
}

void MainMenuController::SetButtonPosition() {
	/*�e�{�^���̈ʒu��ݒ肷��*/
	buttonSizeX = 
		BC.buttonContainer[ButtonEvent::NEW_GAME].ReferGraphSizeX();
	buttonSizeZ =
		BC.buttonContainer[ButtonEvent::NEW_GAME].ReferGraphSizeZ();

	for (int i = ButtonEvent::NEW_GAME; i <= ButtonEvent::GAME_OVER; i++) {
		buttonPosition[i][0] = BUTTON_POSITION::START_POSITION_ON_X;
	}

	buttonPosition[ButtonEvent::NEW_GAME][1] =
		BUTTON_POSITION::NEW_GAME;
	buttonPosition[ButtonEvent::CONTINUE_GAME][1] =
		BUTTON_POSITION::CONTINUE_GAME;
	buttonPosition[ButtonEvent::LOAD_GAME][1] =
		BUTTON_POSITION::LOAD_GAME;
	buttonPosition[ButtonEvent::OPTION][1] =
		BUTTON_POSITION::OPTION;
	buttonPosition[ButtonEvent::GAME_OVER][1] =
		BUTTON_POSITION::GAME_OVER;

	for (int i = ButtonEvent::NEW_GAME; i <= ButtonEvent::GAME_OVER; i++) {
		BC.buttonContainer[i].SetXZ(buttonPosition[i][0],
			buttonPosition[i][1], buttonMultiple);
	}
}

int MainMenuController::CheckChoice(){
	int choice = -1;//�Ԃ��l��NULL�ɐݒ肷��
	bool pressed = false;

	int mouseX, mouseZ;

	/*�}�E�X�̃X�e�[�^�X���擾*/
	GetMousePoint(&mouseX, &mouseZ);
	if (GetMouseInput() == MOUSE_INPUT_LEFT) {
		pressed = true;
	}

	/*�{�^���̃X�e�[�^�X�̓}�E�X�ɍ��킹�ĕύX����*/
	for (int i = ButtonEvent::NEW_GAME; i <= ButtonEvent::GAME_OVER; i++) {
		if (mouseX > buttonPosition[i][0] * Screen::SCREEN_X &&
			mouseX < buttonPosition[i][0] *
			Screen::SCREEN_X + buttonSizeX * buttonMultiple &&
			mouseZ > buttonPosition[i][1] * Screen::SCREEN_Z &&
			mouseZ < buttonPosition[i][1] *
			Screen::SCREEN_Z + buttonSizeZ * buttonMultiple) {

			BC.buttonContainer[i].ChangePressed();//�I�ׂ��{�^���̏�Ԃ�ύX

			if (pressed)//������������R�}���h��Ԃ�
				choice = i;
		}
	}

	return choice;
}

void MainMenuController::FREE() {
	BC.FREE();
	PL.FREE_FOR_MAINMENU();
	SL.FreeAll();
}