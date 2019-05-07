#pragma once
/*このヘッダーファイルに保存しているのはAIの行動指令表*/
enum RadianRange {
	RANGE_MAX = 60,
	RANGE_1_2 = 30,
	RANGE_1_4 = 10,
	RANGE_1_8 = 0,
	SPEED_MAX = 25,
	SPEED_1_2 = 18,
	SPEED_1_4 = 9,
	SPEED_1_8 = 4
};

enum DistanceRange {
	PATROL_RANGE = 4000,//0.5出力
	COMING_IN_RANGE = 4000,//.75
	TAKE_T = 2000,//100
	BATTLE = 1000,//same with target
	CLOSING = 1000,
	SLOW_DOWN = 300,
	STOP = 100
};

enum AI_WAYPOINT {
	ON_X = 500,
	ON_Z = 500
};

enum AI_MOVE_FLAGSHIP{
	PATROL = 0,//パトロール//xunluo
	/*CYCLEの部分は反航戦の時使う*/
	APPROACH_OUT_OF_BATTLE_CYCLE,
	APPROACH_OUT_OF_BATTLE,
	APPROACH_IN_BATTLE_CYCLE,
	APPROACH_IN_BATTLE,
	IN_BATTLE_CYCLE,
	IN_BATTLE
};

enum AI_FLAGSHIP_SPEED {
	BATTLE_SPEED_NORMAL,//普通に戦闘が行う時の速度
	BATTLE_SPEED_ESCAPE_PURSUIT,//逃げる時の速度
	PARTORL //パトロール時の速度
};

enum BATTLE_STATUS {
	APPROACHING,//同航戦
	GOING_FAR_AWAY//反航戦
};

enum AI_FIGHT_FLAGSHIP {
	CONCENTRATE_ON_FLAGSHIP,//敵のフラグシープに集中する
	CONCENTRATE_ON_MY_MARK,//自分のターゲットと同じにする
	FREE_FIRE_KILL_DAMAGED_FIRST,//自由射撃-ダメージを受けた敵を優先目標にする
	FREE_FIRE_SHOOT_INTACT_FIRST,//自由射撃-損傷を受けなかった敵を優先目標にする
	FREE_FIRE_SHOOT_CLOSE,//自由射撃-一番近い目標をターゲットにする
	FREE_FIRE_SHOOT_FAR//自由射撃-一番遠い目標をターゲットにする
};

enum LINE_UP {
	VERTICAL_LAYOUT,//縦型陣
	//横陣
	//斜め陣
};