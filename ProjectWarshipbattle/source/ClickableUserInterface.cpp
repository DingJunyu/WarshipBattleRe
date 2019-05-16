#include "ClickableUserInterface.h"

ClickableUserInterface::~ClickableUserInterface()
{
}

/*メモリ関連操作*/
void ClickableUserInterface::MemorySecure(int Count) {
	buttonPosition = new double *[Count];
	for (int i = 0; i < Count; i++) {
		buttonPosition[i] = new double[7];
		buttonPosition[i][SaveForCUI::SHOW] = 1;
	}
}

void ClickableUserInterface::MemoryFree(int Count) {
	for (int i = 0; i < Count; i++) {
		delete[] buttonPosition[i];
	}
	delete[] buttonPosition;
}

/*初期化関数など*/
void ClickableUserInterface::IngameInif(PictureLoader *PL, SoundLoader *SL) {
	MemorySecure(CommandSerial::COUNT);
	BC.InifForUserInterface(PL);

	for (int i = CommandSerial::TEST_VIEW_ON;
		i <= CommandSerial::SELECT_IN_FORMATION + 16; i++) {
			buttonPosition[i][SaveForCUI::SHOW] = 1;
	}

	/*メニュー部分*/
	SetThisOne(CommandSerial::MENU, BUTTON_POSITION_INGAME::MENU_X,
		BUTTON_POSITION_INGAME::MENU_Z, BUTTON_POSITION_INGAME::MENU_MULTI);
	SetThisOne(CommandSerial::CONTINUE, BUTTON_POSITION_INGAME::CONTINUE_X,
		BUTTON_POSITION_INGAME::CONTINUE_Z, BUTTON_POSITION_INGAME::CONTINUE_MULTI);
	/*SetThisOne(CommandSerial::SETTING, BUTTON_POSITION_INGAME::SETTING_X,
		BUTTON_POSITION_INGAME::SETTING_Z, BUTTON_POSITION_INGAME::SETTING_MULTI);*/
	SetThisOne(CommandSerial::EXIT, BUTTON_POSITION_INGAME::EXIT_X,
		BUTTON_POSITION_INGAME::EXIT_Z, BUTTON_POSITION_INGAME::EXIT_MULTI);

	SetThisOne(CommandSerial::CHANGE_CAMERA, BUTTON_POSITION_INGAME::CHANGE_CAMERA_X,
		BUTTON_POSITION_INGAME::CHANGE_CAMERA_Z, BUTTON_POSITION_INGAME::CHANGE_CAMERA_MULTI);
	SetThisOne(CommandSerial::AUTO_FIRE, 0.8,
		0.8, 0.25);

	/*砲塔操作部分*/
	SetThisOne(CommandSerial::SHOOT, BUTTON_POSITION_INGAME::SHOOT_X,
		BUTTON_POSITION_INGAME::SHOOT_Z, BUTTON_POSITION_INGAME::SHOOT_MULTI);
	SetThisOne(CommandSerial::TURRET_TURN_RIGHT, BUTTON_POSITION_INGAME::TURRET_TURN_RIGHT_X,
		BUTTON_POSITION_INGAME::TURRET_TURN_RIGHT_Z, BUTTON_POSITION_INGAME::TURRET_TURN_RIGHT_MULTI);
	SetThisOne(CommandSerial::TURRET_TURN_LEFT, BUTTON_POSITION_INGAME::TURRET_TURN_LEFT_X,
		BUTTON_POSITION_INGAME::TURRET_TURN_LEFT_Z, BUTTON_POSITION_INGAME::TURRET_TURN_LEFT_MULTI);
	SetThisOne(CommandSerial::TURRET_PULLUP, BUTTON_POSITION_INGAME::TURRET_PULLUP_X,
		BUTTON_POSITION_INGAME::TURRET_PULLUP_Z, BUTTON_POSITION_INGAME::TURRET_PULLUP_MULTI);
	SetThisOne(CommandSerial::TURRET_PULLDOWN, BUTTON_POSITION_INGAME::TURRET_PULLDOWN_X,
		BUTTON_POSITION_INGAME::TURRET_PULLDOWN_Z, BUTTON_POSITION_INGAME::TURRET_PULLDOWN_MULTI);

	SetThisOne(CommandSerial::FORECAST_PLUS, BUTTON_POSITION_INGAME::TURRET_TURN_RIGHT_X,
		BUTTON_POSITION_INGAME::TURRET_TURN_RIGHT_Z, BUTTON_POSITION_INGAME::TURRET_TURN_RIGHT_MULTI);
	SetThisOne(CommandSerial::FORECAST_MINUS, BUTTON_POSITION_INGAME::TURRET_TURN_LEFT_X,
		BUTTON_POSITION_INGAME::TURRET_TURN_LEFT_Z, BUTTON_POSITION_INGAME::TURRET_TURN_LEFT_MULTI);
	SetThisOne(CommandSerial::REVISE_RADIAN_ON_Z_MINUS, BUTTON_POSITION_INGAME::TURRET_PULLUP_X,
		BUTTON_POSITION_INGAME::TURRET_PULLUP_Z, BUTTON_POSITION_INGAME::TURRET_PULLUP_MULTI);
	SetThisOne(CommandSerial::REVISE_RADIAN_ON_Z_PLUS, BUTTON_POSITION_INGAME::TURRET_PULLDOWN_X,
		BUTTON_POSITION_INGAME::TURRET_PULLDOWN_Z, BUTTON_POSITION_INGAME::TURRET_PULLDOWN_MULTI);

	/*出力部分*/
	SetThisOne(CommandSerial::INCREASE_OUTPUT, BUTTON_POSITION_INGAME::INCREASE_OUTPUT_X,
		BUTTON_POSITION_INGAME::INCREASE_OUTPUT_Z, BUTTON_POSITION_INGAME::INCREASE_OUTPUT_MULTI);
	SetThisOne(CommandSerial::DECREASE_OUTPUT, BUTTON_POSITION_INGAME::DECREASE_OUTPUT_X,
		BUTTON_POSITION_INGAME::DECREASE_OUTPUT_Z, BUTTON_POSITION_INGAME::DECREASE_OUTPUT_MULTI);
	
	SetThisOne(CommandSerial::TURN_RIGHT, BUTTON_POSITION_INGAME::TURN_RIGHT_X,
		BUTTON_POSITION_INGAME::TURN_RIGHT_Z, BUTTON_POSITION_INGAME::TURN_RIGHT_MULTI);
	SetThisOne(CommandSerial::TURN_LEFT, BUTTON_POSITION_INGAME::TURN_LEFT_X,
		BUTTON_POSITION_INGAME::TURN_LEFT_Z, BUTTON_POSITION_INGAME::TURN_LEFT_MULTI);
	SetThisOne(CommandSerial::TURN_RETURN, BUTTON_POSITION_INGAME::TURN_RETURN_X,
		BUTTON_POSITION_INGAME::TURN_RETURN_Z, BUTTON_POSITION_INGAME::TURN_RETURN_MULTI);

	/*編成メニュー*/



	for (int i = CommandSerial::INCREASE_OUTPUT;
		i <= CommandSerial::CHOOSE_SUB; i++)
		BC.buttonContainer[i].SetXZ(
			buttonPosition[i][SaveForCUI::COORD_X],
			buttonPosition[i][SaveForCUI::COORD_Z],
			buttonPosition[i][SaveForCUI::MULTIPLE]
		);
}

/*左と右の船リストを初期化する*/
void ClickableUserInterface::InifShipList(std::vector<ShipMain> *shipList, bool left) {
	double count = 0;

	int number;
	
	if (!shipList->empty())
		for (auto ship = shipList->begin();
			ship != shipList->end();
			ship++) {
		/*右リスト・左リストに応じて番号を取る*/
		if (!left)
			number = CommandSerial::SELECT + (int)count + CommandSerial::SELECT_RANGE;
		else
			number = CommandSerial::SELECT + (int)count;


		/*座標を設定する*/
		SetThisOne(number,
			BUTTON_POSITION_INGAME::SHIP_MARK_RIGHT_X,
			BUTTON_POSITION_INGAME::SHIP_MARK_NEXT_Z * (count + 1),
			BUTTON_POSITION_INGAME::SHIP_MARK_MULTI);
		/*設定されたデータをボタンクラスに更新する*/
		BC.buttonContainer[number].SetXZ(
			buttonPosition[number][SaveForCUI::COORD_X],
			buttonPosition[number][SaveForCUI::COORD_Z],
			buttonPosition[number][SaveForCUI::MULTIPLE]);
		buttonPosition[number][SaveForCUI::SHOW] = 0;
		
		count++;
	}
}

void ClickableUserInterface::CheckShipList(std::vector<ShipMain> *shipList, bool left) {

}

void ClickableUserInterface::Free() {
	MemoryFree(CommandSerial::COUNT);
}

int ClickableUserInterface::CheckChoice() {

	int answer = CommandSerial::NONE_COMMAND;
	bool pressed = false;

	int mouseX, mouseZ;

	GetMousePoint(&mouseX, &mouseZ);
	if (GetMouseInput() == MOUSE_INPUT_LEFT) {
		pressed = true;
	}

	for (int i = CommandSerial::INCREASE_OUTPUT;
		i <= CommandSerial::SELECT_IN_FORMATION + 16;
		i++) {
		if(buttonPosition[i][SaveForCUI::SHOW] == 0)
			/*マウスの座標とボタンの範囲を照合する*/
		if (mouseX > buttonPosition[i][SaveForCUI::COORD_X] * Screen::SCREEN_X &&
			mouseX < buttonPosition[i][SaveForCUI::COORD_X] *
			Screen::SCREEN_X + buttonPosition[i][SaveForCUI::SIZE_X] *
			buttonPosition[i][SaveForCUI::MULTIPLE] &&
			mouseZ > buttonPosition[i][SaveForCUI::COORD_Z] * Screen::SCREEN_Z &&
			mouseZ < buttonPosition[i][SaveForCUI::COORD_Z] *
			Screen::SCREEN_Z + buttonPosition[i][SaveForCUI::SIZE_Z] *
			buttonPosition[i][SaveForCUI::MULTIPLE]) {

			BC.buttonContainer[i].ChangePressed();//画像を押された状態に変更

			/*押されたら選択しを返す*/	
			if (pressed) {
				answer = i;
			}
		}
	}

	return answer;//選択したもの対応した番号を返す
}

void ClickableUserInterface::SetThisOne(int num, double x, double z, double mt) {
	buttonPosition[num][SaveForCUI::SIZE_X] = BC.buttonContainer[num].ReferGraphSizeX();
	buttonPosition[num][SaveForCUI::SIZE_Z] = BC.buttonContainer[num].ReferGraphSizeZ();

	buttonPosition[num][SaveForCUI::COORD_X] = x;
	buttonPosition[num][SaveForCUI::COORD_Z] = z;
	buttonPosition[num][SaveForCUI::MULTIPLE] = mt;
	buttonPosition[num][SaveForCUI::SHOW] = 1;
}

void ClickableUserInterface::Draw() {
	/*標準ボタン*/
	for (int i = CommandSerial::INCREASE_OUTPUT;
		i <= CommandSerial::MINUS; i++) {
		if (buttonPosition[i][SaveForCUI::SHOW] == 0)
			BC.buttonContainer[i].DrawThisButton();
	}
	/*敵選択ボタン*/
	for (int i = CommandSerial::SELECT;
		i <= CommandSerial::SELECT + CommandSerial::SELECT_RANGE * 2;
		i++) {
		if (buttonPosition[i][SaveForCUI::SHOW] == 0)
			BC.buttonContainer[i].DrawThisButton();
	}
	/*編成ボタン*/
	for (int i = CommandSerial::SELECT_IN_FORMATION;
		i < CommandSerial::SELECT_IN_FORMATION + 16; i++) {
		if (buttonPosition[i][SaveForCUI::SHOW] == 0)
			BC.buttonContainer[i].DrawThisButton();
	}
}

void ClickableUserInterface::SetFormationMenuStatus() {

	SetThisOne(CommandSerial::GAME_START, 0.365, .9, 0.75);
	BC.buttonContainer[CommandSerial::GAME_START].SetXZ(
		buttonPosition[CommandSerial::GAME_START][SaveForCUI::COORD_X],
		buttonPosition[CommandSerial::GAME_START][SaveForCUI::COORD_Z],
		buttonPosition[CommandSerial::GAME_START][SaveForCUI::MULTIPLE]);

	int realNumPlus;
	int realNumMinus;
	int realNumPlusRight;
	int realNumMinusRight;

	for (int i = 0; i < 8; i += 2) {
		realNumPlus = CommandSerial::SELECT_IN_FORMATION + i;
		realNumMinus = CommandSerial::SELECT_IN_FORMATION + i + 1;
		realNumPlusRight = realNumPlus + 8;
		realNumMinusRight = realNumMinus + 8;
		SetThisOne(realNumPlus, 0.45, 0.07 + .079*(i + 1), .1);
		SetThisOne(realNumMinus, 0.45, 0.13 + .079*(i + 1), .1);
		SetThisOne(realNumPlusRight, 0.94, 0.07 + .079*(i + 1), .1);
		SetThisOne(realNumMinusRight, 0.94, 0.13 + .079*(i + 1), .1);

		BC.buttonContainer[realNumPlus].SetXZ(
			buttonPosition[realNumPlus][SaveForCUI::COORD_X],
			buttonPosition[realNumPlus][SaveForCUI::COORD_Z],
			buttonPosition[realNumPlus][SaveForCUI::MULTIPLE]
		);
		BC.buttonContainer[realNumMinus].SetXZ(
			buttonPosition[realNumMinus][SaveForCUI::COORD_X],
			buttonPosition[realNumMinus][SaveForCUI::COORD_Z],
			buttonPosition[realNumMinus][SaveForCUI::MULTIPLE]
		);
		BC.buttonContainer[realNumPlusRight].SetXZ(
			buttonPosition[realNumPlusRight][SaveForCUI::COORD_X],
			buttonPosition[realNumPlusRight][SaveForCUI::COORD_Z],
			buttonPosition[realNumPlusRight][SaveForCUI::MULTIPLE]
		);
		BC.buttonContainer[realNumMinusRight].SetXZ(
			buttonPosition[realNumMinusRight][SaveForCUI::COORD_X],
			buttonPosition[realNumMinusRight][SaveForCUI::COORD_Z],
			buttonPosition[realNumMinusRight][SaveForCUI::MULTIPLE]
		);
	}



	buttonPosition[CommandSerial::GAME_START][SaveForCUI::SHOW] = 0;

	for (int i = CommandSerial::SELECT_IN_FORMATION;
		i <= CommandSerial::SELECT_IN_FORMATION + 16; i++) {
		buttonPosition[i][SaveForCUI::SHOW] = 0;
	}
}

void ClickableUserInterface::CloseFormationMenu() {
	buttonPosition[CommandSerial::GAME_START][SaveForCUI::SHOW] = 1;

	for (int i = CommandSerial::SELECT_IN_FORMATION;
		i <= CommandSerial::SELECT_IN_FORMATION + 16; i++) {
		buttonPosition[i][SaveForCUI::SHOW] = 1;
	}

	SetNormalStatus();
}

/*初めてボタンを初期化時表すべきボタンをオンにする*/
void ClickableUserInterface::SetNormalStatus() {
	buttonPosition[CommandSerial::MENU][SaveForCUI::SHOW] = 0;
	buttonPosition[CommandSerial::SHOOT][SaveForCUI::SHOW] = 0;
	buttonPosition[CommandSerial::INCREASE_OUTPUT][SaveForCUI::SHOW] = 0;
	buttonPosition[CommandSerial::DECREASE_OUTPUT][SaveForCUI::SHOW] = 0;
	buttonPosition[CommandSerial::TURN_RIGHT][SaveForCUI::SHOW] = 0;
	buttonPosition[CommandSerial::TURN_LEFT][SaveForCUI::SHOW] = 0;
	buttonPosition[CommandSerial::TURN_RETURN][SaveForCUI::SHOW] = 0;
	buttonPosition[CommandSerial::TURRET_TURN_RIGHT][SaveForCUI::SHOW] = 0;
	buttonPosition[CommandSerial::TURRET_TURN_LEFT][SaveForCUI::SHOW] = 0;
	buttonPosition[CommandSerial::TURRET_PULLUP][SaveForCUI::SHOW] = 0;
	buttonPosition[CommandSerial::TURRET_PULLDOWN][SaveForCUI::SHOW] = 0;
}

void ClickableUserInterface::LetMeSeeMenu() {
	menuOpened = !menuOpened;

	int choice = menuOpened ? 0 : 1;

	for (int i = CommandSerial::CONTINUE; i <= CommandSerial::EXIT; i++) {
		buttonPosition[i][SaveForCUI::SHOW] = choice;
	}
}

void ClickableUserInterface::CloseMenu() {
	for (int i = CommandSerial::CONTINUE; i <= CommandSerial::EXIT; i++) {
		buttonPosition[i][SaveForCUI::SHOW] = 1;
	}
	menuOpened = false;
}

void ClickableUserInterface::SetShootMenu(bool status) {
	int choice = status ? 1 : 0; //ロックがオンの時に消します
	for (int i = CommandSerial::TURRET_TURN_RIGHT;
		i <= CommandSerial::TURRET_PULLDOWN; i++) {
		buttonPosition[i][SaveForCUI::SHOW] = choice;
	}
	choice = status ? 0 : 1;
	for (int i = CommandSerial::FORECAST_PLUS;
		i <= CommandSerial::REVISE_RADIAN_ON_Z_MINUS; i++) {
		buttonPosition[i][SaveForCUI::SHOW] = choice;
	}
}

void ClickableUserInterface::DrawNeedInput(bool lock, int num) {
	DrawChoosedShip(lock, num);
}

void ClickableUserInterface::DrawChoosedShip(bool lock, int num) {
	if (!lock)
		return;

	int buttonNum = num + CommandSerial::SELECT + CommandSerial::SELECT_RANGE;

	Coordinate2D<int> coord = BC.buttonContainer[buttonNum].ReferCoord();

	DrawBoxAA((float)(coord.x),
		(float)(coord.z),
		(float)(coord.x + BC.buttonContainer[buttonNum].ReferGraphSizeX()*
		BC.buttonContainer[buttonNum].ReferZoomMultiple()),
		(float)(coord.z + BC.buttonContainer[buttonNum].ReferGraphSizeZ()*
		BC.buttonContainer[buttonNum].ReferZoomMultiple()),
		GetColor(255, 0, 0), FALSE, 2.0f
	);
}