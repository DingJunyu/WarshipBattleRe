#pragma once
namespace Screen {
	const int SCREEN_X = 1280;
	const int SCREEN_Z = 760;
	const int COLOR = 32;
	const double MULTIPLE = (double)SCREEN_X / 1280.0;
	const double DEFAULT_MULTIPLE = 1.0;
	const double MAX_MULTIPLE = 1.5;
	const double MIN_MULTIPLE = 0.4;
}

//コントローラー用指令集
enum CommandSerial {
	COUNT = 100,//コマンドの数
	NONE_COMMAND = -1,//命令なし
	TEST_VIEW_ON = 0,//デバッグモードon
	INCREASE_OUTPUT = 1,//出力増加
	DECREASE_OUTPUT,//出力減少
	TURN_RIGHT,//舵を右に回る
	TURN_LEFT,//舵を左に回る
	TURN_RETURN,//舵を中心に戻る
	SHOOT,//射撃
	TURRET_TURN_RIGHT,
	TURRET_TURN_LEFT,
	TURRET_PULLUP,
	TURRET_PULLDOWN,
	FORECAST_PLUS,
	FORECAST_MINUS,
	CHANGE_CAMERA,
	AUTO_FIRE,
	REVISE_RADIAN_ON_Z_PLUS,
	REVISE_RADIAN_ON_Z_MINUS,
	MENU,
	CONTINUE,
//	SETTING,
	EXIT,
	GAME_START,
	RANDOM_FORMATION,
	CLEAR_FORMATION,
	PLUS,
	MINUS,
//	FORMATION_UP,
//	FORMATION_DOWN,
//	FORMATION_RIGHT,
//	FORMATION_LEFT,
	CHOOSE_MAIN,
	CHOOSE_SUB,


	SELECT,/*いつも最後に設置する*/

	TEST_INCREASE_FRAME,
	TEST_DECREASE_FRAME,

	SELECT_RANGE = 10,

	SELECT_IN_FORMATION = 60,//編成用

	/*編成メニュー新オーダー*/
	EXIT_IN_FORMATION = 76,

	/*ZoomIn、ZoomOut*/
	ZOOM_IN = 200,
	ZOOM_OUT = 201,
	GET_DAMAGE_TEST
};

//CUIの中に保存されたものとその番号
enum SaveForCUI {
	COORD_X = 0,
	COORD_Z,
	SIZE_X,
	SIZE_Z,
	MULTIPLE,
	SHOW
};

enum TutorialNum {
	TN_AMOUNT = 6,
	DIRECT_CONTROL = 0,
	SPEED_CONTROL,
	WEAPON_CONTROL,
	FORMATION,
	INBATTLE_NORMAL, 
	INBATTLE_LOCKON
};

namespace AUTO_SHIP_DATA {
	const int AROUND_POINT = 32;
}

//船型の番号
const int RESERVED_NUMBER_FOR_SHIPS = 1000;
enum ShipType {
	DESTROYER = 1,//1
	LIGHT_CRUISER,//2
	HEAVEY_CRUISER,//3
	BATTLE_CRUISER,//4
	BATTLESHIP,//5
	AIRCRAFT_CARRIER,//6
	LIGHT_AIRCRAFT_CARRIER,//7
	CVE//8
};

//カメラ種類番号
enum CameraType {
	MAIN = 0,
	SHIP = 1,
	AMMO,
	TORPEDO,
	BOMB,
	PLANE
};

enum Discoverd {
	DISCOVERD = 0,//名前まで知る
	APPROACH,//どの型まで知る
	DETECTED,//種類を知る
	UNDETECTED //全く知らない
};

//飛行機の種類定義
enum PlaneType {
	FIGHTER = 1,
	BOMBER,
	ATTACKER
};

//航空武器の定義
enum PlaneWeaponType {
	BOMB_PW = 1,
	TORPEDO_PW,
	MACHINE_GUN_PW
};

enum PlaneStatus {
	FLYING = 401,
	FIGHTING,
	TAKINGOFF,
	LANDING,
	FALLING,
	SPEEDUP,
	SPEEDDOWN
};

enum SerialNumberOfShipHandle {
	SHIP_HANDLE = 0,
	SHIP_SHADOW_HANDLE,
	SHIP_SIDEVIEW_HANDLE,
	SHIP_SINKING,
	SHIP_CARD,
	ARRAY_SIZE = 20
};

enum SerialNumberOfMapObjectHandle {
	MO_HANDLE = 0,
	MO_MINI_VER_HANDLE,
	MO_SHADOW_HANDLE,
	MO_SIZE_X,
	MO_SIZE_Z
};

enum TypeOfEffect {
	EFFECT_AMOUNT = 5,
	EXPLOSION = 0,
	BUBBLE,
	SMOKE,
	RIPPLE,
	EFFECT_FLAME_AMOUNT = 16 
};

enum TypeOfHord {
	LARGE,
	MEDIUM,
	SMALL
};

enum FormationBoard {
	FB_NUM = 4,
	FB_BACKGROUND = 0,
	FB_SHIP_CARD,
	FB_FLAG
};

enum StatisticBoard {
	SB_NUM = 5,
	SB_BACKGROUND = 0,
	WIN,
	LOSE,
	NEW_LIGHT,
	NEW_DARK
};

enum ButtonEvent {
	BUTTON_AMOUNT = 30,
	//メインメニューボタン集
	NEW_GAME = 0,
	TUTORIAL,
	GAME_OVER,
	TTR_DIRECT_CONTROL,
	TTR_SPEED_CONTROL,
	TTR_WEAPON_CONTROL,
	TTR_FORMATION,
	TTR_INBATTLE_NORMAL,
	TTR_INBATTLE_LOCKON,
	TTR_BACK,
	/*これ以下の部分はゲーム中に使っていない*/
	CONTINUE_GAME,
	LOAD_GAME,
	OPTION,

};

enum UI_LIST {
	AMOUNT = 30,

	RADAR = 0,
	FORECAST_BUTTON,
	FORECAST_POINT,
	ARROW_RED,
	ARROW_RED_2,
	ARROW_WHITE,
	RUDDER,
	SHOW_RADIAN_ON_Y,
	SHOW_DIRECT,
	TEXT_BAR_DIRECT,
	TEXT_BAR_SPEED,
	TEXT_BAR_TURRET,
	SPEED_PAN,
	ENGINE_PAN,
	LOCK_CIRCLE,
	MY_DIRECT,
	UI_LOCK_ON,
	UI_AUTO_FIRE,
	CAMERA_ON_ME,
	CAMERA_ON_ENEMY,
	GAME_WIN,
	GAME_LOSE
};

enum LAMP_LIST {
	LAMP_AMOUNT = 2,

	RETURN_TO_MIDDLE = 0,
	LOCKED
};

enum VIDEO_LIST {
	VIDEO_AMOUNT = 1,
	TITLE = 0
};

namespace BUTTON_POSITION {
	const double START_POSITION_ON_X = .2;
	const double NEW_GAME = .4;
	const double CONTINUE_GAME = .5;
	const double LOAD_GAME = .6;
	const double OPTION = .7;
	const double GAME_OVER = .8;
}

namespace BUTTON_POSITION_INGAME {
	const double MENU_MULTI = 0.08;
	const double MENU_X = 0.85;
	const double MENU_Z = 0.02;

	const double CONTINUE_MULTI = 0.08;
	const double CONTINUE_X = 0.85;
	const double CONTINUE_Z = 0.08;

	const double SETTING_MULTI = 0.08;
	const double SETTING_X = .85;
	const double SETTING_Z = .14;

	const double EXIT_MULTI = .08;
	const double EXIT_X = .85;
	const double EXIT_Z = .14;

	const double SHOOT_MULTI = .24;
	const double SHOOT_X = .63;
	const double SHOOT_Z = .81;

	const double TURRET_TURN_LEFT_MULTI = .06;
	const double TURRET_TURN_LEFT_X = .63;
	const double TURRET_TURN_LEFT_Z = .91;
	
	const double TURRET_TURN_RIGHT_MULTI = .06;
	const double TURRET_TURN_RIGHT_X = .705;
	const double TURRET_TURN_RIGHT_Z = .91;

	const double TURRET_PULLUP_MULTI = .06;
	const double TURRET_PULLUP_X = .63;
	const double TURRET_PULLUP_Z = .94;

	const double TURRET_PULLDOWN_MULTI = .06;
	const double TURRET_PULLDOWN_X = .705;
	const double TURRET_PULLDOWN_Z = .94;

	const double TURN_RIGHT_MULTI = .06;
	const double TURN_RIGHT_X = 0.5 + .03;
	const double TURN_RIGHT_Z = .7;

	const double TURN_LEFT_MULTI = .06;
	const double TURN_LEFT_X = 0.5 - .08;
	const double TURN_LEFT_Z = .7;

	const double TURN_RETURN_MULTI = .06;
	const double TURN_RETURN_X = 0.5 - .027;
	const double TURN_RETURN_Z = .7;

	const double INCREASE_OUTPUT_MULTI = .24;
	const double INCREASE_OUTPUT_X = 0.025;
	const double INCREASE_OUTPUT_Z = 0.79;
	
	const double DECREASE_OUTPUT_MULTI = .24;
	const double DECREASE_OUTPUT_X = 0.025;
	const double DECREASE_OUTPUT_Z = 0.88;

	const double CHANGE_CAMERA_MULTI = .4;
	const double CHANGE_CAMERA_X = 0.83;
	const double CHANGE_CAMERA_Z = .6;

	const double SHIP_MARK_MULTI = .13;
	const double SHIP_MARK_LEFT_X = .002;
	const double SHIP_MARK_LEFT_X_COORD = SHIP_MARK_LEFT_X * Screen::SCREEN_X;
	const double SHIP_MARK_RIGHT_X = .943;
	const double SHIP_MARK_RIGHT_X_COORD = SHIP_MARK_RIGHT_X * Screen::SCREEN_X;
	const double SHIP_MARK_START_Z = .02;
	const double SHIP_MARK_NEXT_Z = .04;
	const double SHIP_MARK_NEXT_Z_COORD = SHIP_MARK_NEXT_Z * Screen::SCREEN_Z;
}

namespace USER_INTERFACE_POSITION {
	const double RADAR_A_MULTI = .04;
	const double RADAR_A_X = .705;
	const double RADAR_A_Z = .79;

	const double ARROW_RED_A_MULTI = .036;
	const double ARROW_RED_A_X = RADAR_A_X + .0278;
	const double ARROW_RED_A_Z = RADAR_A_Z + .0445;

	const double ARROW_WHITE_SPEED_MULTI = .08;
	const double ARROW_WHITE_SPEED_X = .14;
	const double ARROW_WHITE_SPEED_Z = .88;

	const double ARROW_WHITE_ENGINE_MULTI = .08;
	const double ARROW_WHITE_ENGINE_X = .26;
	const double ARROW_WHITE_ENGINE_Z = .88;

	const double SHOOT_CD_MAIN_X = .707;
	const double SHOOT_CD_MAIN_Z = .886;
	const int SHOOT_CD_MAIN_SIZE_X = 63;
	const int SHOOT_CD_MAIN_SIZE_Z = 16;

	const double RUDDER_MULTI = .07;
	const double RUDDER_X = .5;
	const double RUDDER_Z = .7;

	const double SHOW_RADIAN_ON_Y_MULTI = .1;
	const double SHOW_RADIAN_ON_Y_X = .686;
	const double SHOW_RADIAN_ON_Y_Z = .89;

	const double SHOW_DIRECT_MULTI = .1;
	const double SHOW_DIRECT_X = .5;
	const double SHOW_DIRECT_Z = .639;

	const double TEXT_BAR_DIRECT_MULTI = .065;
	const double TEXT_BAR_DIRECT_X = .5;
	const double TEXT_BAR_DIRECT_Z = .745;

	const double TEXT_BAR_SPEED_MULTI = .06;
	const double TEXT_BAR_SPEED_X = .0265;
	const double TEXT_BAR_SPEED_Z = .765;

	const double TEXT_BAR_TURRET_MULTI = .065;
	const double TEXT_BAR_TURRET_X = .665;
	const double TEXT_BAR_TURRET_Z = .76;

	const double SPEED_PAN_MULTI = .08;
	const double SPEED_PAN_X = .14;
	const double SPEED_PAN_Z = .88;

	const double ENGINE_PAN_MULTI = .08;
	const double ENGINE_PAN_X = .26;
	const double ENGINE_PAN_Z = .88;

	const double LOCK_CIRCLE = .4;
	const double MY_DIRECT_MULTI = .5;
}

namespace LAMP_POSITION {
	const double RETURN_TO_MIDDLE_X = 0.5;
	const double RETURN_TO_MIDDLE_Z = 0.735;
	const double RETURN_TO_MIDDLE_R = 4;

	const double LOCKED_X = .655;
	const double LOCKED_Z = .8;
	const double LOCKED_R = 5;
}

enum AmmoType {
	TYPE_COUNT = 4,
	SMALL_CALIBER = 0,
	MEDIUM_CALIBER,
	HEAVY_CALIBER,
	HUGE_CALIBER
};

struct StatisticData {

	StatisticData() {
		shootCount = 0;
		hitCount = 0;
		hitRate = 0;
		movedDis = 0;
		damage = 0;
		damageRecieved = 0;
		killed = 0;
	}

	int shootCount;
	int hitCount;
	double hitRate;
	double movedDis;
	int damage;
	int damageRecieved;
	int killed;
};

struct Choice {
	int num;
	bool plus;
};

/*3D座標保存*/
template <typename T>
struct Coordinate {
	T x;
	T y;
	T z;
};

template<typename T>
struct CrashSize3d {
	T x;//長さ
	T y;//縦の長さ
	T z;//横の長さ
};

/*2D座標保存*/
template <typename T>
struct Coordinate2D {
	Coordinate2D() { x = 0; z = 0; }
	Coordinate2D(T X, T Z) : x(X), z(Z) {}

	T x;
	T z;

	/*インターフェイス*/
	void ChangeX() { x = -x; }
	void ChangeZ() { z = -z; }
	void ChangeXandZ() { x = -x; z = -z; }

	Coordinate2D<T> operator+(const Coordinate2D<T> &b) {
		Coordinate2D<T> coord;
		coord.x = this->x + b.x;
		coord.z = this->z + b.z;
		return coord;
	}
};

struct RadianNeededIn3D {
	double z;//平面角度
	double y;//縦面角度
};

struct FiringData {
	bool isThisMainWeapon;
	bool selected[50];
};

enum FIRE_DISTANCE {
	ANGLE_TYPE_AMOUNT = 6,
	ANGLE_100_P = 0,
	ANGLE_80_P,
	ANGLE_60_P,
	ANGLE_40_P,
	ANGLE_20_P,
	ANGLE_0_P
};

/*各種類の船の数*/
namespace COUNT_OF_EVERYTYPE {
	const int ALL_SHIP_COUNT = 9000;
	const int DESTROYER_TYPE_COUNT = 0;
	const int LIGHT_CRUSIER_TYPE_COUNT = 0;
	const int HEAVEY_CRUSIER_TYPE_COUNT = 0;
	const int BATTLE_CRUSIER_TYPE_COUNT = 1;
	const int BATTLESHIP_TYPE_COUNT = 0;
	const int AIRCRAFT_CARRIER_TYPE_COUNT = 0;
	const int LIGHT_AIRCRAFT_CARRIER_TYPE_COUNT = 0;
	const int CVE_TYPE_COUNT = 0;
}

/*船の番号*/
namespace ShipSerial {
	const int BATTLE_CRUSIER_LVWZOV = 4000;
}

/*数学&物理定義*/
namespace MathAndPhysics {
	const double GRAVITATIONAL_ACCELERATION = 0.0980665;
	const int GRAVITATIONAL_ACCELERATION_INT = 10;
	const double MIN_AIR_RESISTANCE = 0.0005;
	const double PI = 3.14159265358;
	const double OneDegree = 1.0 / 180.0;
	const double OneDegreeRadian = PI * OneDegree;
	const double MAX_SPEED_FOR_SHIP = 1.30;
	const double RANGE_ERROR = 10;//角度探す関数が正常に動作しない時に適度に上がってみよう
	const double Infinite = 99999999.0;

	const double ShipCardRate = 0.2;
	const double FantasyNumRate = 0.2;

	const double Change_Distance = 47;
}

namespace TIME_NEEDED {
	const int ONE_MINUTE = 60000;
}

namespace GameDefine {
	const int MAX_UNIT_A_TEAM = 10;
}

namespace MapSize {
	const int xSize = 20000;
	const int zSize = 20000;
	const int safeZone = 1000;
}

namespace RandomDataWhenShoot {
	const double radianOnZ = MathAndPhysics::PI * (1.0 / 180.0);
	const double radianOnY = MathAndPhysics::PI * (1.0 / 180.0);
	const double speed = 2;
}

struct GameDataStatisticsForEveryShip {
	int shoot;
	int hit;
	int kill;
};