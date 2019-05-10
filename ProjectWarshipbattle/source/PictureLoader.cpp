#include "PictureLoader.h"

PictureLoader::~PictureLoader()
{
}

void PictureLoader::MemorySecure() {
	
	shipHandle = new int *[COUNT_OF_EVERYTYPE::ALL_SHIP_COUNT];
	for (int i = 0;
		i < COUNT_OF_EVERYTYPE::ALL_SHIP_COUNT;
		i++) {
		shipHandle[i] 
			= new int [SerialNumberOfShipHandle::ARRAY_SIZE];
	}

	//ボタン
	buttonHandle = new int[ButtonEvent::BUTTON_AMOUNT];
	buttonPressedHandle = new int[ButtonEvent::BUTTON_AMOUNT];

	//弾
	ammoHandle = new int[AmmoType::TYPE_COUNT];
	ammoX = new int[AmmoType::TYPE_COUNT];
	ammoZ = new int[AmmoType::TYPE_COUNT];
}

void PictureLoader::FreeMemory() {
	for (int i = 0;
		i < COUNT_OF_EVERYTYPE::ALL_SHIP_COUNT;
		i++) {
		delete[] shipHandle[i];
	}
	delete[] shipHandle;

	//ボタン
	delete[] buttonHandle;
	delete[] buttonPressedHandle;

	//弾
	delete[] ammoHandle;
	delete[] ammoX;
	delete[] ammoZ;
}

/********************************************************************/
/*2019.4.9画像読み込み部分を軽量化するため、関数に分けました*/
/********************************************************************/
void PictureLoader::LoadPicture() {
	LoadShips();
	LoadSea();
	LoadUserInterfaceInGame();
	LoadEffect();
	LoadAmmo();
	LoadMainMenu();
	LoadButtonInMainMenu();
	LoadButtonInGame();
}

void PictureLoader::LoadShips() {
	/*ロード戦闘巡洋艦*/
	shipHandle[4000][SerialNumberOfShipHandle::SHIP_HANDLE] =
		LoadGraph(
			"GraphicData/Ship_TopView/BattleCrusier/BattleCrusier_TopView_00_Lvtzow.png");
	GetGraphSize(shipHandle[4000][SerialNumberOfShipHandle::SHIP_HANDLE],
		&shipSizeX, &shipSizeZ);
	shipHandle[4000][SerialNumberOfShipHandle::SHIP_SHADOW_HANDLE] =
		LoadGraph(
			"GraphicData/Ship_Shadow/BattleCrusier/BattleCrusier_Shadow_01_Lvtzow.png");
	shipHandle[4000][SerialNumberOfShipHandle::SHIP_CARD] =
		LoadGraph(
			"GraphicData/Interactive_User_Interface/Ships_Mark/Battle_Crusier/BattleCrusier_Mark_001_Gneisenau.png");

	/*ロード戦艦*/
	shipHandle[5000][SerialNumberOfShipHandle::SHIP_HANDLE] =
		LoadGraph(
			"GraphicData/Ship_TopView/BattleShip/BattleShip_TopView_00_Yamashiro.png");
	shipHandle[5000][SerialNumberOfShipHandle::SHIP_SHADOW_HANDLE] =
		LoadGraph(
			"GraphicData/Ship_Shadow/BattleShip/BattleShip_Shadow_00_Yamashiro.png");
}

void PictureLoader::LoadSea() {
	/*海*/
	mapHandle =
		LoadGraph("GraphicData/Sea/Sea.jpg");
	GetGraphSize(mapHandle, &mapX, &mapZ);
}

void PictureLoader::LoadUserInterfaceInGame() {
	/*UI*/
	matHandle = LoadGraph(
		"GraphicData/User_Interface/UI_Mat.jpg");
	GetGraphSize(matHandle, &matHandleX, &matHandleZ);
	shipCardMatHandle = LoadGraph(
		"GraphicData/User_Interface/Ship_Card_Mat.jpg");
	GetGraphSize(shipCardMatHandle, &shipCardMatHandleX, &shipCardMatHandleZ);
	UIList[UI_LIST::RADAR] = LoadGraph(
		"GraphicData/User_Interface/Radar.png");
	UIList[UI_LIST::FORECAST_BUTTON] = LoadGraph(
		"GraphicData/User_Interface/knob.png");
	UIList[UI_LIST::FORECAST_POINT] = LoadGraph(
		"GraphicData/User_Interface/pointOnKnob.png");
	UIList[UI_LIST::ARROW_RED] = LoadGraph(
		"GraphicData/User_Interface/arrow_red.png");
	UIList[UI_LIST::ARROW_RED_2] = LoadGraph(
		"GraphicData/User_Interface/arrow_red_2.png");
	UIList[UI_LIST::ARROW_WHITE] = LoadGraph(
		"GraphicData/User_Interface/arrow_white.png");
	UIList[UI_LIST::RUDDER] = LoadGraph(
		"GraphicData/User_Interface/rudder.png");
	UIList[UI_LIST::SHOW_RADIAN_ON_Y] = LoadGraph(
		"GraphicData/User_Interface/ShowRadianOnY.png");
	UIList[UI_LIST::SHOW_DIRECT] = LoadGraph(
		"GraphicData/User_Interface/ShowDirect.png");
	UIList[UI_LIST::TEXT_BAR_DIRECT] = LoadGraph(
		"GraphicData/User_Interface/Text_Bar_Direct.png");
	UIList[UI_LIST::TEXT_BAR_SPEED] = LoadGraph(
		"GraphicData/User_Interface/Text_Bar_Speed.png");
	UIList[UI_LIST::TEXT_BAR_TURRET] = LoadGraph(
		"GraphicData/User_Interface/Text_Bar_Turret.png");
	UIList[UI_LIST::SPEED_PAN] = LoadGraph(
		"GraphicData/User_Interface/Speed_Pan.png");
	UIList[UI_LIST::ENGINE_PAN] = LoadGraph(
		"GraphicData/User_Interface/Engine_Pan.png");
	UIList[UI_LIST::LOCK_CIRCLE] = LoadGraph(
		"GraphicData/User_Interface/Lock.png");
	UIList[UI_LIST::MY_DIRECT] = LoadGraph(
		"GraphicData/User_Interface/ShowPlayerDirect.png");

	/*ミニマップ*/
	miniMapMatHandle = LoadGraph(
		"GraphicData/User_Interface/MiniMap_Mat.jpg");
	miniMapPaperHandle = LoadGraph(
		"GraphicData/User_Interface/MiniMap_Paper.jpg");
}

void PictureLoader::LoadEffect() {
	/*エフェクト*/
	bubbleHandle = LoadGraph(
		"GraphicData/Effect/Bubble.png");
	smokeHandle = LoadGraph(
		"GraphicData/Effect/Smoke.png");
	effectHandleList[TypeOfEffect::EXPLOSION] = LoadGraph(
		"GraphicData/Effect/explosion.png");
}

void PictureLoader::LoadAmmo() {
	//弾
	ammoHandle[0] = LoadGraph(
		"GraphicData/Shell/Shell.png");
}

void PictureLoader::LoadMainMenu() {
	//タイトル関係
	title = LoadGraph(
		"GraphicData/Back_Ground/Title.png");
	pressedToStart = LoadGraph(
		"GraphicData/Back_Ground/Press_To_Start.png");
	pressedToStartDown = LoadGraph(
		"GraphicData/Back_Ground/Press_To_Start_Down.png");
	backGround = LoadGraph(
		"GraphicData/Back_Ground/Title_MainMenu_BackGround.jpg");
}

void PictureLoader::LoadButtonInMainMenu() {
	//ボタン
    //メインメニュー部分
	buttonHandle[ButtonEvent::NEW_GAME] = LoadGraph(
		"GraphicData/Button/Button_NewGame.png");
	buttonPressedHandle[ButtonEvent::NEW_GAME] = LoadGraph(
		"GraphicData/Button/Button_Pressed_NewGame.png");
	buttonHandle[ButtonEvent::CONTINUE_GAME] = LoadGraph(
		"GraphicData/Button/Button_ContinueGame.png");
	buttonPressedHandle[ButtonEvent::CONTINUE_GAME] = LoadGraph(
		"GraphicData/Button/Button_Pressed_ContinueGame.png");
	buttonHandle[ButtonEvent::LOAD_GAME] = LoadGraph(
		"GraphicData/Button/Button_LoadGame.png");
	buttonPressedHandle[ButtonEvent::LOAD_GAME] = LoadGraph(
		"GraphicData/Button/Button_Pressed_LoadGame.png");
	buttonHandle[ButtonEvent::OPTION] = LoadGraph(
		"GraphicData/Button/Button_Option.png");
	buttonPressedHandle[ButtonEvent::OPTION] = LoadGraph(
		"GraphicData/Button/Button_Pressed_Option.png");
	buttonHandle[ButtonEvent::GAME_OVER] = LoadGraph(
		"GraphicData/Button/Button_EndGame.png");
	buttonPressedHandle[ButtonEvent::GAME_OVER] = LoadGraph(
		"GraphicData/Button/Button_Pressed_EndGame.png");
}

void PictureLoader::LoadButtonInGame() {
	//ゲーム中身の部分
	buttonIntheGame[CommandSerial::INCREASE_OUTPUT] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/speed_up_normal.png");
	buttonPressedIntheGame[CommandSerial::INCREASE_OUTPUT] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/speed_up_pressed.png");
	buttonIntheGame[CommandSerial::DECREASE_OUTPUT] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/speed_down_normal.png");
	buttonPressedIntheGame[CommandSerial::DECREASE_OUTPUT] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/speed_down_pressed.png");

	buttonIntheGame[CommandSerial::TURN_RIGHT] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/ship_turn_right_normal.png");
	buttonPressedIntheGame[CommandSerial::TURN_RIGHT] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/ship_turn_right_pressed.png");
	buttonIntheGame[CommandSerial::TURN_LEFT] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/ship_turn_left_normal.png");
	buttonPressedIntheGame[CommandSerial::TURN_LEFT] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/ship_turn_left_pressed.png");
	buttonIntheGame[CommandSerial::TURN_RETURN] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/ship_return_to_middle_normal.png");
	buttonPressedIntheGame[CommandSerial::TURN_RETURN] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/ship_return_to_middle_pressed.png");

	buttonIntheGame[CommandSerial::SHOOT] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/shoot_normal.png");
	buttonPressedIntheGame[CommandSerial::SHOOT] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/shoot_pressed.png");
	buttonIntheGame[CommandSerial::TURRET_TURN_RIGHT] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/turn_right_normal.png");
	buttonPressedIntheGame[CommandSerial::TURRET_TURN_RIGHT] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/turn_right_pressed.png");
	buttonIntheGame[CommandSerial::TURRET_TURN_LEFT] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/turn_left_normal.png");
	buttonPressedIntheGame[CommandSerial::TURRET_TURN_LEFT] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/turn_left_pressed.png");
	buttonIntheGame[CommandSerial::TURRET_PULLUP] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/pull_up_normal.png");
	buttonPressedIntheGame[CommandSerial::TURRET_PULLUP] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/pull_up_pressed.png");
	buttonIntheGame[CommandSerial::TURRET_PULLDOWN] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/pull_down_normal.png");
	buttonPressedIntheGame[CommandSerial::TURRET_PULLDOWN] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/pull_down_pressed.png");

	buttonIntheGame[CommandSerial::FORECAST_PLUS] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/plus_1_second.png");
	buttonPressedIntheGame[CommandSerial::FORECAST_PLUS] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/plus_1_second_pressed.png");
	buttonIntheGame[CommandSerial::FORECAST_MINUS] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/minus_1_second.png");
	buttonPressedIntheGame[CommandSerial::FORECAST_MINUS] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/minus_1_second_pressed.png");
	buttonIntheGame[CommandSerial::REVISE_RADIAN_ON_Z_PLUS] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/revise_on_y_plus.png");
	buttonPressedIntheGame[CommandSerial::REVISE_RADIAN_ON_Z_PLUS] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/revise_on_y_plus_pressed.png");
	buttonIntheGame[CommandSerial::REVISE_RADIAN_ON_Z_MINUS] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/revise_on_y_minus.png");
	buttonPressedIntheGame[CommandSerial::REVISE_RADIAN_ON_Z_MINUS] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/revise_on_y_minus_pressed.png");

	buttonIntheGame[CommandSerial::CHANGE_CAMERA] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/change_camera.png");
	buttonPressedIntheGame[CommandSerial::CHANGE_CAMERA] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/change_camera_pressed.png");

	buttonIntheGame[CommandSerial::MENU] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/menu_normal.png");
	buttonPressedIntheGame[CommandSerial::MENU] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/menu_pressed.png");
	buttonIntheGame[CommandSerial::CONTINUE] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/continue_normal.png");
	buttonPressedIntheGame[CommandSerial::CONTINUE] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/continue_pressed.png");
	buttonIntheGame[CommandSerial::SETTING] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/option_normal.png");
	buttonPressedIntheGame[CommandSerial::SETTING] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/option_pressed.png");
	buttonIntheGame[CommandSerial::EXIT] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/endgame_normal.png");
	buttonPressedIntheGame[CommandSerial::EXIT] = LoadGraph(
		"GraphicData/Interactive_User_Interface/buttons/endgame_pressed.png");

	buttonIntheGame[CommandSerial::SELECT] = LoadGraph(
		"GraphicData/Interactive_User_Interface/Ships_Mark/Ship_Unknown.png");

	/*編成メニューに使うやつ*/
	buttonIntheGame[CommandSerial::GAME_START] = LoadGraph(
		"GraphicData/Interactive_User_Interface/GameMenu/start_button.png");
	buttonPressedIntheGame[CommandSerial::GAME_START] = LoadGraph(
		"GraphicData/Interactive_User_Interface/GameMenu/start_button_pressed.png");
	buttonIntheGame[CommandSerial::PLUS] = LoadGraph(
		"GraphicData/Interactive_User_Interface/GameMenu/plus.png");
	buttonPressedIntheGame[CommandSerial::PLUS] = LoadGraph(
		"GraphicData/Interactive_User_Interface/GameMenu/plus_pressed.png");
	buttonIntheGame[CommandSerial::MINUS] = LoadGraph(
		"GraphicData/Interactive_User_Interface/GameMenu/minus.png");
	buttonPressedIntheGame[CommandSerial::MINUS] = LoadGraph(
		"GraphicData/Interactive_User_Interface/GameMenu/minus_pressed.png");
	buttonIntheGame[CommandSerial::FORMATION_UP] = LoadGraph(
		"GraphicData/Interactive_User_Interface/GameMenu/up.png");
	buttonPressedIntheGame[CommandSerial::FORMATION_UP] = LoadGraph(
		"GraphicData/Interactive_User_Interface/GameMenu/up.png");
	buttonIntheGame[CommandSerial::FORMATION_DOWN] = LoadGraph(
		"GraphicData/Interactive_User_Interface/GameMenu/down.png");
	buttonPressedIntheGame[CommandSerial::FORMATION_DOWN] = LoadGraph(
		"GraphicData/Interactive_User_Interface/GameMenu/down.png");
	buttonIntheGame[CommandSerial::FORMATION_RIGHT] = LoadGraph(
		"GraphicData/Interactive_User_Interface/GameMenu/right.png");
	buttonPressedIntheGame[CommandSerial::FORMATION_RIGHT] = LoadGraph(
		"GraphicData/Interactive_User_Interface/GameMenu/right.png");
	buttonIntheGame[CommandSerial::FORMATION_LEFT] = LoadGraph(
		"GraphicData/Interactive_User_Interface/GameMenu/left.png");
	buttonPressedIntheGame[CommandSerial::FORMATION_LEFT] = LoadGraph(
		"GraphicData/Interactive_User_Interface/GameMenu/left.png");

}

void PictureLoader::LoadStatisticBoard() {
	statisticBoardHandle[StatisticBoard::SB_BACKGROUND] = LoadGraph(
		"GraphicData/Back_Ground/statistic_board.jpg");
	statisticBoardHandle[StatisticBoard::WIN] = LoadGraph(
		"GraphicData/Back_Ground/win.png");
	statisticBoardHandle[StatisticBoard::LOSE] = LoadGraph(
		"GraphicData/Back_Ground/lose.png");
}

void PictureLoader::LoadFormationBoard() {
	formationBoardHandle[FormationBoard::FB_BACKGROUND] = LoadGraph(
		"GraphicData/Back_Ground/team_menu.jpg");
	formationBoardHandle[FormationBoard::FB_SHIP_CARD] = LoadGraph(
		"GraphicData/Back_Ground/Ship_Card.png");
}

void PictureLoader::FreePicture() {
	DeleteShips();
	DeleteSea();
	DeleteUserInterfaceInGame();
	DeleteEffect();
	DeleteAmmo();
	DeleteMainMenu();
	DeleteButtonInMainMenu();
	DeleteButtonInGame();
}

void PictureLoader::DeleteShips() {
	/*戦艦*/
	for (int i = 0;
		i < COUNT_OF_EVERYTYPE::BATTLE_CRUSIER_TYPE_COUNT;
		i++) {
		DeleteGraph(
			shipHandle[i][SerialNumberOfShipHandle::SHIP_HANDLE]);
		DeleteGraph(
			shipHandle[i][SerialNumberOfShipHandle::SHIP_SHADOW_HANDLE]);
	}
}

void PictureLoader::DeleteSea() {
	/*マップ*/
	DeleteGraph(mapHandle);
}

void PictureLoader::DeleteUserInterfaceInGame() {
	/*UI*/
	DeleteGraph(matHandle);
	DeleteGraph(shipCardMatHandle);
	DeleteGraph(miniMapMatHandle);
	DeleteGraph(miniMapPaperHandle);
	for (int i = 0; i < UI_LIST::AMOUNT; i++) {
		DeleteGraph(UIList[i]);
	}
}

void PictureLoader::DeleteEffect(){
	/*エフェクト*/
	DeleteGraph(bubbleHandle);
	DeleteGraph(smokeHandle);
}

void PictureLoader::DeleteAmmo() {
	DeleteGraph(ammoHandle[0]);
}

void PictureLoader::DeleteMainMenu() {
	DeleteGraph(title);
	DeleteGraph(pressedToStart);
	DeleteGraph(pressedToStartDown);
	DeleteGraph(backGround);
}

void PictureLoader::DeleteButtonInMainMenu() {
	/*ボタン*/
	for (int i = 0; i < ButtonEvent::BUTTON_AMOUNT; i++) {
		DeleteGraph(buttonHandle[i]);
		DeleteGraph(buttonPressedHandle[i]);
	}
}

void PictureLoader::DeleteButtonInGame() {
	for (int i = CommandSerial::INCREASE_OUTPUT;
		i <= CommandSerial::COUNT; i++) {
		DeleteGraph(buttonIntheGame[i]);
		DeleteGraph(buttonPressedIntheGame[i]);
	}
}

void PictureLoader::DeleteStatisticBoard() {
	for (int i = StatisticBoard::SB_BACKGROUND;
		i <= StatisticBoard::LOSE;
		i++)
		DeleteGraph(statisticBoardHandle[i]);
}