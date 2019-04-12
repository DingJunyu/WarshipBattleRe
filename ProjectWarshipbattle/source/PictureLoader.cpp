#include "PictureLoader.h"

PictureLoader::~PictureLoader()
{
}

void PictureLoader::MemorySecure() {
	
	battleCruiserHandle
		= new int *[COUNT_OF_EVERYTYPE::BATTLE_CRUSIER_TYPE_COUNT];
	for (int i = 0;
		i < COUNT_OF_EVERYTYPE::BATTLE_CRUSIER_TYPE_COUNT;
		i++) {
		battleCruiserHandle[i] 
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
		i < COUNT_OF_EVERYTYPE::BATTLE_CRUSIER_TYPE_COUNT;
		i++) {
		delete[] battleCruiserHandle[i];
	}
	delete[] battleCruiserHandle;

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
	battleCruiserHandle[0][SerialNumberOfShipHandle::SHIP_HANDLE] =
		LoadGraph(
			"GraphicData/Ship_TopView/BattleCrusier/BattleCrusier_TopView_00_Lvtzow.png");
	GetGraphSize(battleCruiserHandle[0][SerialNumberOfShipHandle::SHIP_HANDLE],
		&shipSizeX, &shipSizeZ);
	battleCruiserHandle[0][SerialNumberOfShipHandle::SHIP_SHADOW_HANDLE] =
		LoadGraph(
			"GraphicData/Ship_Shadow/BattleCrusier/BattleCrusier_Shadow_01_Lvtzow.png");
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
			battleCruiserHandle[i][SerialNumberOfShipHandle::SHIP_HANDLE]);
		DeleteGraph(
			battleCruiserHandle[i][SerialNumberOfShipHandle::SHIP_SHADOW_HANDLE]);
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