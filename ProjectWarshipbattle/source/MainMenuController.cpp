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
	//使うやつのサイズを取る
	GetGraphSize(*title, &titleSizeX, &titleSizeZ);
	GetGraphSize(*pressedToStart, &PTSSizeX, &PTSSizeZ);

	SetButtonPosition();//各ボタンの位置を設定する
}

void MainMenuController::DrawTitle() {
	SetDrawScreen(DX_SCREEN_BACK);//裏画面に描画する
	
	while (1) {
		ClearDrawScreen();

		/*背景を描画する*/
		DrawExtendGraph(0, 0, Screen::SCREEN_X, Screen::SCREEN_Z,
			*backGroundHandle, FALSE);
		/*タイトルを描画する*/
		DrawExtendGraph(
			Screen::SCREEN_X / 2 - titleSizeX / 4,
			(int)(0.1*Screen::SCREEN_Z),
			Screen::SCREEN_X / 2 + titleSizeX / 4,
			(int)(0.1*Screen::SCREEN_Z + titleSizeZ / 2),
			*title,
			TRUE
		);

		//３０フレームごとに点滅する
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

		/*画像輸出*/
		FC.Wait();
		ScreenFlip();

		countForTitle++;//30フレームごとに点滅する//0.5秒
		countForTitle %= 60;//オーバーフロー対策

		//インプットがあれば終わる
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

	SetDrawScreen(DX_SCREEN_BACK);//裏画面に描画する
	ClearDrawScreen();

	/*背景を描画*/
	DrawExtendGraph(0, 0, Screen::SCREEN_X, Screen::SCREEN_Z,
		*backGroundHandle, FALSE);

	/*ボタンを描画*/
	for (int i = ButtonEvent::NEW_GAME; i <= ButtonEvent::GAME_OVER; i++) {
		BC.buttonContainer[i].DrawThisButton();
	}

	/*タイトルを描画*/
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
	/*各ボタンの位置を設定する*/
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
	int choice = -1;//返す値をNULLに設定する
	bool pressed = false;

	int mouseX, mouseZ;

	/*マウスのステータスを取得*/
	GetMousePoint(&mouseX, &mouseZ);
	if (GetMouseInput() == MOUSE_INPUT_LEFT) {
		pressed = true;
	}

	/*ボタンのステータスはマウスに合わせて変更する*/
	for (int i = ButtonEvent::NEW_GAME; i <= ButtonEvent::GAME_OVER; i++) {
		if (mouseX > buttonPosition[i][0] * Screen::SCREEN_X &&
			mouseX < buttonPosition[i][0] *
			Screen::SCREEN_X + buttonSizeX * buttonMultiple &&
			mouseZ > buttonPosition[i][1] * Screen::SCREEN_Z &&
			mouseZ < buttonPosition[i][1] *
			Screen::SCREEN_Z + buttonSizeZ * buttonMultiple) {

			BC.buttonContainer[i].ChangePressed();//選べたボタンの状態を変更

			if (pressed)//もし押したらコマンドを返す
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