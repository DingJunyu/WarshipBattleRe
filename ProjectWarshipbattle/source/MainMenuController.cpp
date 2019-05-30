#include "MainMenuController.h"

MainMenuController::~MainMenuController()
{
}

void MainMenuController::Inif() {
	StandardInif();

	pressedToStart = PL.ReferPressedToStart();
	pressedToStartDown = PL.ReferPressedToStartDown();
	GetGraphSize(*pressedToStart, &PTSSizeX, &PTSSizeZ);

	SetButtonPosition();//各ボタンの位置を設定する
}

void MainMenuController::Inif_Tur() {
	StandardInif();

	tutorialHandle = PL.ReferTutorialHandle();

	SetButtonPos_Tur();
}

void MainMenuController::StandardInif() {
	PL.InifForMainMenu();
	SL.Inif();
	BC.Inif(&PL);

	backGroundHandle = PL.ReferBackGround();
	title = PL.ReferTitle();

	//使うやつのサイズを取る
	GetGraphSize(*title, &titleSizeX, &titleSizeZ);

	for (int i = 0; i < ButtonEvent::BUTTON_AMOUNT; i++) {
		buttonPosition[i][BC_ACTIVE] = false;
	}
}

void MainMenuController::FREE() {
	BC.FREE();
	PL.FREE_FOR_MAINMENU();
	SL.FreeAll();

	/*ポインターを解放する*/
	title = nullptr;
	backGroundHandle = nullptr;
	pressedToStart = nullptr;
	pressedToStartDown = nullptr;
}

void MainMenuController::DrawTitle() {
	SetDrawScreen(DX_SCREEN_BACK);//裏画面に描画する

	SetUseASyncLoadFlag(TRUE);//非同期読み込みを有効化
	VideoHandle = LoadGraph(".//Video//Game_View.mpg");
	SetUseASyncLoadFlag(FALSE);//非同期読み込みを無効化
	
	
	while (1) {
		ClearDrawScreen();

		if (GetASyncLoadNum() == 0 && Loading) {
			MoviePlaying = PlayMovieToGraph(VideoHandle);
			Loading = false;
		}

		if (GetMovieStateToGraph(VideoHandle) != 1 && !Loading) {
			SeekMovieToGraph(VideoHandle,0);
			MoviePlaying = PlayMovieToGraph(VideoHandle);
		}

		/*背景を描画する*/
		if (MoviePlaying != 0)
			DrawExtendGraph(0, 0, Screen::SCREEN_X, Screen::SCREEN_Z,
				*backGroundHandle, FALSE);
		else
			DrawGraph(0, 0, VideoHandle, FALSE);

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
			|| GetMouseInput() == MOUSE_INPUT_LEFT && GetASyncLoadNum() == 0)
			break;

		if (ProcessMessage() == -1)
			break;
	}

	PauseMovieToGraph(VideoHandle);
	DeleteGraph(VideoHandle);
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

void MainMenuController::DrawTutorial() {
	if (firstTimeGetIntoMainMenu) {
		FC.Reset();
		firstTimeGetIntoMainMenu = false;
	}

	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	DrawExtendGraph(0, 0, Screen::SCREEN_X, Screen::SCREEN_Z,
		*backGroundHandle, FALSE);

	SetTrans(180);

	DrawBox(0, 0, 150, 768, CrBox.ReferColor(LOCHMARA), TRUE);
	DrawBox(0, 0, 145, 768, CrBox.ReferColor(BLUE_CHARCOAL), TRUE);

	ResetTrans();

	DrawTutorialSingle();

	for (int i = ButtonEvent::TTR_DIRECT_CONTROL; 
		i <= ButtonEvent::TTR_BACK; i++) {
		BC.buttonContainer[i].DrawThisButton();
	}
	FC.Wait();

	ScreenFlip();
}

void MainMenuController::SetButtonPosition() {
	/*各ボタンの位置を設定する*/
	buttonSizeX = 
		BC.buttonContainer[ButtonEvent::NEW_GAME].ReferGraphSizeX();
	buttonSizeZ =
		BC.buttonContainer[ButtonEvent::NEW_GAME].ReferGraphSizeZ();

	for (int i = ButtonEvent::NEW_GAME; i <= ButtonEvent::GAME_OVER; i++) {
		buttonPosition[i][BC_X] = BUTTON_POSITION::START_POSITION_ON_X;
	}

	buttonPosition[ButtonEvent::NEW_GAME][BC_Z] =
		BUTTON_POSITION::NEW_GAME;
	buttonPosition[ButtonEvent::TUTORIAL][BC_Z] =
		BUTTON_POSITION::CONTINUE_GAME;
	buttonPosition[ButtonEvent::GAME_OVER][BC_Z] =
		BUTTON_POSITION::GAME_OVER;

	for (int i = ButtonEvent::NEW_GAME; i <= ButtonEvent::GAME_OVER; i++) {
		BC.buttonContainer[i].SetXZ(buttonPosition[i][BC_X],
			buttonPosition[i][BC_Z], buttonMultiple);
		buttonPosition[i][BC_ACTIVE] = true;
	}
}

void MainMenuController::SetButtonPos_Tur() {
	/*各ボタンの位置を設定する*/
	buttonSizeX =
		BC.buttonContainer[ButtonEvent::NEW_GAME].ReferGraphSizeX();
	buttonSizeZ =
		BC.buttonContainer[ButtonEvent::NEW_GAME].ReferGraphSizeZ();

	for (int i = ButtonEvent::TTR_DIRECT_CONTROL;
		i <= ButtonEvent::TTR_INBATTLE_LOCKON; i++) {
		buttonPosition[i][BC_X] = 0.005;
		buttonPosition[i][BC_Z] = 0.07 * (i - 2);//1から始まるようにする
		BC.buttonContainer[i].SetXZ(buttonPosition[i][BC_X],
			buttonPosition[i][BC_Z], buttonMultiple_TTR);
		buttonPosition[i][BC_ACTIVE] = true;
	}

	buttonPosition[ButtonEvent::TTR_BACK][BC_X] = 0.005;
	buttonPosition[ButtonEvent::TTR_BACK][BC_Z] = 0.85;
	BC.buttonContainer[ButtonEvent::TTR_BACK].SetXZ(
		buttonPosition[ButtonEvent::TTR_BACK][BC_X],
		buttonPosition[ButtonEvent::TTR_BACK][BC_Z], buttonMultiple_TTR);
	buttonPosition[ButtonEvent::TTR_BACK][BC_ACTIVE] = true;
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
	for (int i = ButtonEvent::NEW_GAME; i <= ButtonEvent::TTR_BACK; i++) {
		if (buttonPosition[i][BC_ACTIVE])
			if (mouseX > buttonPosition[i][BC_X] * Screen::SCREEN_X &&
				mouseX < buttonPosition[i][BC_X] *
				Screen::SCREEN_X + buttonSizeX * buttonMultiple &&
				mouseZ > buttonPosition[i][BC_Z] * Screen::SCREEN_Z &&
				mouseZ < buttonPosition[i][BC_Z] *
				Screen::SCREEN_Z + buttonSizeZ * buttonMultiple)
			{

				BC.buttonContainer[i].ChangePressed();//選べたボタンの状態を変更

				if (pressed)//もし押したらコマンドを返す
					choice = i;
			}
	}

	return choice;
}

void MainMenuController::DrawTutorialSingle() {
	DrawExtendGraph(150, 50,1280, 743,
		*(tutorialHandle + TTR_choice), TRUE);
}