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

	for (int i = CommandSerial::INCREASE_OUTPUT; 
		i <= CommandSerial::SELECT + CommandSerial::SELECT_RANGE; i++) {
		for (int j = 0; j < 6; j++)
			buttonPosition[i][j] = 1;
	}

	/*メニュー部分*/
	SetThisOne(CommandSerial::MENU, BUTTON_POSITION_INGAME::MENU_X,
		BUTTON_POSITION_INGAME::MENU_Z, BUTTON_POSITION_INGAME::MENU_MULTI);
	SetThisOne(CommandSerial::CONTINUE, BUTTON_POSITION_INGAME::CONTINUE_X,
		BUTTON_POSITION_INGAME::CONTINUE_Z, BUTTON_POSITION_INGAME::CONTINUE_MULTI);
	SetThisOne(CommandSerial::SETTING, BUTTON_POSITION_INGAME::SETTING_X,
		BUTTON_POSITION_INGAME::SETTING_Z, BUTTON_POSITION_INGAME::SETTING_MULTI);
	SetThisOne(CommandSerial::EXIT, BUTTON_POSITION_INGAME::EXIT_X,
		BUTTON_POSITION_INGAME::EXIT_Z, BUTTON_POSITION_INGAME::EXIT_MULTI);

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


	for (int i = CommandSerial::INCREASE_OUTPUT;
		i <= CommandSerial::EXIT; i++)
		BC.buttonContainer[i].SetXZ(
			buttonPosition[i][SaveForCUI::COORD_X],
			buttonPosition[i][SaveForCUI::COORD_Z],
			buttonPosition[i][SaveForCUI::MULTIPLE]
		);
}

void ClickableUserInterface::InifShipList(std::vector<ShipMain> *shipList, bool left) {
	double count = 0;
	
	if (!shipList->empty())
		for (auto ship = shipList->begin();
			ship != shipList->end();
			ship++) {
		if (!left) {
			SetThisOne(CommandSerial::SELECT + (int)count + CommandSerial::SELECT_RANGE,
				BUTTON_POSITION_INGAME::SHIP_MARK_RIGHT_X,
				BUTTON_POSITION_INGAME::SHIP_MARK_NEXT_Z * (count + 1),
				BUTTON_POSITION_INGAME::SHIP_MARK_MULTI);
			BC.buttonContainer[SELECT + (int)count + CommandSerial::SELECT_RANGE].SetXZ(
				buttonPosition[SELECT + (int)count + CommandSerial::SELECT_RANGE][SaveForCUI::COORD_X],
				buttonPosition[SELECT + (int)count + CommandSerial::SELECT_RANGE][SaveForCUI::COORD_Z],
				buttonPosition[SELECT + (int)count + CommandSerial::SELECT_RANGE][SaveForCUI::MULTIPLE]);
			buttonPosition[SELECT + (int)count + CommandSerial::SELECT_RANGE][SaveForCUI::SHOW] = 0;
		}
		else {
			SetThisOne(CommandSerial::SELECT + (int)count,
				BUTTON_POSITION_INGAME::SHIP_MARK_LEFT_X,
				BUTTON_POSITION_INGAME::SHIP_MARK_NEXT_Z * (count + 1),
				BUTTON_POSITION_INGAME::SHIP_MARK_MULTI);
			BC.buttonContainer[SELECT + (int)count].SetXZ(
				buttonPosition[SELECT + (int)count][SaveForCUI::COORD_X],
				buttonPosition[SELECT + (int)count][SaveForCUI::COORD_Z],
				buttonPosition[SELECT + (int)count][SaveForCUI::MULTIPLE]);
			buttonPosition[SELECT + (int)count][SaveForCUI::SHOW] = 0;
		}
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
		i <= CommandSerial::SELECT + CommandSerial::SELECT_RANGE * 2;
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

	return answer;
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
	for (int i = CommandSerial::INCREASE_OUTPUT;
		i <= CommandSerial::SELECT + CommandSerial::SELECT_RANGE * 2; i++) {
		if (buttonPosition[i][SaveForCUI::SHOW] == 0)
			BC.buttonContainer[i].DrawThisButton();
	}
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
	for (int i = CommandSerial::CONTINUE; i <= CommandSerial::EXIT; i++) {
		buttonPosition[i][SaveForCUI::SHOW] = 0;
	}
	menuOpened = true;
}

void ClickableUserInterface::CloseMenu() {
	for (int i = CommandSerial::CONTINUE; i <= CommandSerial::EXIT; i++) {
		buttonPosition[i][SaveForCUI::SHOW] = 1;
	}
	menuOpened = false;
}
