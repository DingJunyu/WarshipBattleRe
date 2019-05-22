#include "UserInterface.h"
using namespace DxLib;

UserInterface::~UserInterface()
{
}

void UserInterface::InifUI(PictureLoader *PL) {
	//画像読み込む
	mat = PL->ReferMatHandle();
	shipCardMat = PL->ReferShipCardMatHandle();
	miniMapMat = PL->ReferMiniMapMatHandle();
	miniMapPaper = PL->ReferMiniMapPaperHandle();

	for (int i = UI_LIST::RADAR; i <= UI_LIST::CAMERA_ON_ENEMY; i++)
		handleList[i] = PL->ReferUIList(i);//ハンドルをコピーする

	Coordinate2D<float> coord;
	unsigned int color;
	//電球初期化
	//座標を計算する
	coord.x = (float)(Screen::SCREEN_X)*(float)(LAMP_POSITION::RETURN_TO_MIDDLE_X);
	coord.z = (float)(Screen::SCREEN_Z)*(float)(LAMP_POSITION::RETURN_TO_MIDDLE_Z);
	//座標を電球に代入
	lamps[LAMP_LIST::RETURN_TO_MIDDLE].Inif(coord,
		(float)LAMP_POSITION::RETURN_TO_MIDDLE_R);
	//ロック状態の指示
	coord.x = (float)(Screen::SCREEN_X)*(float)(LAMP_POSITION::LOCKED_X);
	coord.z = (float)(Screen::SCREEN_Z)*(float)(LAMP_POSITION::LOCKED_Z);
	lamps[LAMP_LIST::LOCKED].Inif(coord,
		(float)LAMP_POSITION::LOCKED_R);
	//オフ状態の色を設定
	color = GetColor(255, 0, 0);
	lamps[LAMP_LIST::RETURN_TO_MIDDLE].SetColor(false, color);
	lamps[LAMP_LIST::LOCKED].SetColor(false, color);
	//オン状態の色を設定
	color = GetColor(0, 255, 0);
	lamps[LAMP_LIST::RETURN_TO_MIDDLE].SetColor(true, color);
	lamps[LAMP_LIST::LOCKED].SetColor(true, color);
}

void UserInterface::DrawUI() {
	/*汎用変数*/
	unsigned int Cr;//色

	/*舵*/
	DrawPictureByCenterOnX(UI_LIST::SHOW_DIRECT,
		USER_INTERFACE_POSITION::SHOW_DIRECT_X,
		USER_INTERFACE_POSITION::SHOW_DIRECT_Z,
		USER_INTERFACE_POSITION::SHOW_DIRECT_MULTI);

	//描く順番を注意！
	/*飾り部分*/
	DrawExtendGraph(0, 0, Screen::SCREEN_X / 20, Screen::SCREEN_Z,
		*shipCardMat, FALSE);
	DrawExtendGraph(Screen::SCREEN_X * 19/20,0 ,Screen::SCREEN_X, 
		Screen::SCREEN_Z,
		*shipCardMat, FALSE);
	DrawExtendGraph((int)matStartX, Screen::SCREEN_Z - (int)matStartZ,
		Screen::SCREEN_X - 1, Screen::SCREEN_Z - 1, *mat, FALSE);

	/*ミニマップ*/
	DrawExtendGraph(Screen::SCREEN_X / 2 - (int)mapMatSize,
		Screen::SCREEN_Z - (int)(1.2 * mapMatSize),
		Screen::SCREEN_X / 2 + (int)mapMatSize ,
		Screen::SCREEN_Z, *miniMapMat, FALSE);

	DrawExtendGraph(Screen::SCREEN_X / 2 - (int)mapPaperSize,
		Screen::SCREEN_Z - (int)(1.2 * (int)mapPaperSize),
		Screen::SCREEN_X/2 + (int)mapPaperSize,
		Screen::SCREEN_Z - 5, *miniMapPaper, FALSE);

	/*角度指示*/
	DrawPictureByLeftUp(UI_LIST::SHOW_RADIAN_ON_Y,
		USER_INTERFACE_POSITION::SHOW_RADIAN_ON_Y_X,
		USER_INTERFACE_POSITION::SHOW_RADIAN_ON_Y_Z,
		USER_INTERFACE_POSITION::SHOW_RADIAN_ON_Y_MULTI);

	/*武器CD*/
	Cr = GetColor(0, 0, 0);
	DrawBox(
		(int)(USER_INTERFACE_POSITION::SHOOT_CD_MAIN_X * Screen::SCREEN_X),
		(int)(USER_INTERFACE_POSITION::SHOOT_CD_MAIN_Z * Screen::SCREEN_Z),
		(int)(USER_INTERFACE_POSITION::SHOOT_CD_MAIN_X * Screen::SCREEN_X) +
		(int)USER_INTERFACE_POSITION::SHOOT_CD_MAIN_SIZE_X,
		(int)(USER_INTERFACE_POSITION::SHOOT_CD_MAIN_Z * Screen::SCREEN_Z) +
		(int)USER_INTERFACE_POSITION::SHOOT_CD_MAIN_SIZE_Z,
		Cr,TRUE
	);

	/*速度計*/
	DrawPictureByCenter(UI_LIST::SPEED_PAN,
		USER_INTERFACE_POSITION::SPEED_PAN_X,
		USER_INTERFACE_POSITION::SPEED_PAN_Z,
		USER_INTERFACE_POSITION::SPEED_PAN_MULTI);

	/*出力計*/
	DrawPictureByCenter(UI_LIST::ENGINE_PAN,
		USER_INTERFACE_POSITION::ENGINE_PAN_X,
		USER_INTERFACE_POSITION::ENGINE_PAN_Z,
		USER_INTERFACE_POSITION::ENGINE_PAN_MULTI);

	/*テキストバー*/
	/*方向指示*/
	DrawPictureByCenterOnX(UI_LIST::TEXT_BAR_DIRECT,
		USER_INTERFACE_POSITION::TEXT_BAR_DIRECT_X,
		USER_INTERFACE_POSITION::TEXT_BAR_DIRECT_Z,
		USER_INTERFACE_POSITION::TEXT_BAR_DIRECT_MULTI);


	/*速度指示*/
	DrawPictureByLeftUp(UI_LIST::TEXT_BAR_SPEED,
		USER_INTERFACE_POSITION::TEXT_BAR_SPEED_X,
		USER_INTERFACE_POSITION::TEXT_BAR_SPEED_Z,
		USER_INTERFACE_POSITION::TEXT_BAR_SPEED_MULTI);

	/*砲塔操作指示*/
	DrawPictureByLeftUp(UI_LIST::TEXT_BAR_TURRET,
		USER_INTERFACE_POSITION::TEXT_BAR_TURRET_X,
		USER_INTERFACE_POSITION::TEXT_BAR_TURRET_Z,
		USER_INTERFACE_POSITION::TEXT_BAR_TURRET_MULTI);
}

void UserInterface::DrawUINeedInput(ShipMain *ship, bool autoFire,
	bool changeCamera) {
	/*汎用変数*/
	int graphSizeX, graphSizeZ;//画像サイズ
	int startX, startZ;//座標
	unsigned int Cr;//色

	double radian;

	DrawPictureByCenterOnX(changeCamera ? UI_LIST::CAMERA_ON_ENEMY :
		UI_LIST::CAMERA_ON_ME,
		0.5, 0, 0.25
	);


	/*ロック使ってない状態*/
	if (!ship->fireDataFigureUp.ReferLockOn()) {
		/*レーダーA*/
		DrawPictureByLeftUp(UI_LIST::RADAR,
			USER_INTERFACE_POSITION::RADAR_A_X,
			USER_INTERFACE_POSITION::RADAR_A_Z,
			USER_INTERFACE_POSITION::RADAR_A_MULTI);
		/*レーダーの上の矢印*/
		radian = ship->ReferMainWeaponRadianOnZ() + ship->ReferRadianOnZ()
			- MathAndPhysics::PI / 4;
		DrawRotatePicture(UI_LIST::ARROW_RED,
			USER_INTERFACE_POSITION::ARROW_RED_A_X,
			USER_INTERFACE_POSITION::ARROW_RED_A_Z,
			USER_INTERFACE_POSITION::ARROW_RED_A_MULTI,
			radian);
	}
	/*ロックを使っている状態*/
	else {
		/*knob*/
		DrawPictureByLeftUp(UI_LIST::FORECAST_BUTTON,
			USER_INTERFACE_POSITION::RADAR_A_X,
			USER_INTERFACE_POSITION::RADAR_A_Z,
			USER_INTERFACE_POSITION::RADAR_A_MULTI);
		/*点*/
		radian = ship->ReferForecastSecond() * MathAndPhysics::PI * 
			MathAndPhysics::OneDegree * 6
			- MathAndPhysics::PI * 0.75;
		DrawRotatePicture(UI_LIST::FORECAST_POINT,
			USER_INTERFACE_POSITION::ARROW_RED_A_X,
			USER_INTERFACE_POSITION::ARROW_RED_A_Z,
			USER_INTERFACE_POSITION::ARROW_RED_A_MULTI,
			radian);
		lamps[LAMP_LIST::LOCKED].SetOnOrOff(ship->ReferCanIShoot());
		lamps[LAMP_LIST::LOCKED].Draw();
		/*射撃可能マーク*/
		if (ship->ReferCanIShoot()) {
			DrawPictureByCenter(UI_LIST::UI_LOCK_ON,
				0.69, 0.69, 0.4);
		}
		/**/
		if (autoFire) {
			DrawPictureByLeftUp(UI_LIST::UI_AUTO_FIRE,
				0.84, 0.8, 0.4);
		}
	}

	/*砲塔角度指示*/
	Cr = GetColor(255, 0, 0);//色を設定する
	GetGraphSize(*handleList[UI_LIST::SHOW_RADIAN_ON_Y], &graphSizeX, &graphSizeZ);
	double Percentage;
	Percentage = 1 -
		ship->ReferMainWeaponRadianOnY() / ship->ReferMainMaxWeaponRadianOnY();
	Percentage *= graphSizeZ * USER_INTERFACE_POSITION::SHOW_RADIAN_ON_Y_MULTI;
	//マークの位置を計算する

	startX = (int)(Screen::SCREEN_X * USER_INTERFACE_POSITION::SHOW_RADIAN_ON_Y_X);
	startZ = (int)(Screen::SCREEN_Z * USER_INTERFACE_POSITION::SHOW_RADIAN_ON_Y_Z)
		+ (int)Percentage;
	if (startZ >= Screen::SCREEN_Z * USER_INTERFACE_POSITION::SHOW_RADIAN_ON_Y_Z 
		+ graphSizeZ * USER_INTERFACE_POSITION::SHOW_RADIAN_ON_Y_MULTI - 4)
		startZ = (int)(Screen::SCREEN_Z * USER_INTERFACE_POSITION::SHOW_RADIAN_ON_Y_Z 
		+ graphSizeZ * USER_INTERFACE_POSITION::SHOW_RADIAN_ON_Y_MULTI - 4);
	DrawBox(startX, startZ,
		(int)(startX + graphSizeX * USER_INTERFACE_POSITION::SHOW_RADIAN_ON_Y_MULTI),
		(int)(startZ + 4),
		Cr,TRUE);

	/*武器CD*/
	Cr = GetColor(0, 255, 0);//色を設定する
	int realX;
	if (ship->ReferMainWeaponCD() >= ship->ReferMainWeaponCoolDownTime())
		realX = USER_INTERFACE_POSITION::SHOOT_CD_MAIN_SIZE_X;
	else
		realX = (int)(((double)ship->ReferMainWeaponCD() /
		(double)ship->ReferMainWeaponCoolDownTime())*
			USER_INTERFACE_POSITION::SHOOT_CD_MAIN_SIZE_X);

	DrawBox(
		(int)(USER_INTERFACE_POSITION::SHOOT_CD_MAIN_X * Screen::SCREEN_X),
		(int)(USER_INTERFACE_POSITION::SHOOT_CD_MAIN_Z * Screen::SCREEN_Z),
		(int)(USER_INTERFACE_POSITION::SHOOT_CD_MAIN_X * Screen::SCREEN_X) +
		realX,
		(int)(USER_INTERFACE_POSITION::SHOOT_CD_MAIN_Z * Screen::SCREEN_Z) +
		(int)USER_INTERFACE_POSITION::SHOOT_CD_MAIN_SIZE_Z,
		Cr, TRUE
	);

	/*舵状態*/
	radian = ship->ReferChangingRadian() + MathAndPhysics::PI * 1.25;
	DrawRotatePicture(UI_LIST::RUDDER,
		USER_INTERFACE_POSITION::RUDDER_X,
		USER_INTERFACE_POSITION::RUDDER_Z,
		USER_INTERFACE_POSITION::RUDDER_MULTI,
		radian);

	/*速度指示*/
	radian = MathAndPhysics::PI * 0.65 +//はじめの位置
		(ship->ReferSpeedOnZ() / MathAndPhysics::MAX_SPEED_FOR_SHIP) *
		MathAndPhysics::PI * 1.35 * 1.5;
	DrawRotatePicture(UI_LIST::ARROW_WHITE,
		USER_INTERFACE_POSITION::ARROW_WHITE_SPEED_X,
		USER_INTERFACE_POSITION::ARROW_WHITE_SPEED_Z,
		USER_INTERFACE_POSITION::ARROW_WHITE_SPEED_MULTI,
		radian);

	/*出力指示*/
	radian = MathAndPhysics::PI * 0.65 + //ここは正しい角度を開始するための角度
		abs(ship->ReferOutPutRate() / 1.0) *
		MathAndPhysics::PI * 1.12;//表示範囲を指定し、角度を計算する
	DrawRotatePicture(UI_LIST::ARROW_RED_2,
		USER_INTERFACE_POSITION::ARROW_WHITE_ENGINE_X,
		USER_INTERFACE_POSITION::ARROW_WHITE_ENGINE_Z,
		USER_INTERFACE_POSITION::ARROW_WHITE_ENGINE_MULTI,
		radian);

	radian = MathAndPhysics::PI * 0.65 +
		abs(ship->ReferEngineOutput() / ship->ReferEngineMaxOutput()) *
		MathAndPhysics::PI * 1.12;//後退しても出力を表示できるようにする
	DrawRotatePicture(UI_LIST::ARROW_WHITE,
		USER_INTERFACE_POSITION::ARROW_WHITE_ENGINE_X,
		USER_INTERFACE_POSITION::ARROW_WHITE_ENGINE_Z,
		USER_INTERFACE_POSITION::ARROW_WHITE_ENGINE_MULTI,
		radian);

	/*自動戻るモード状態の提示*/
	lamps[LAMP_LIST::RETURN_TO_MIDDLE].SetOnOrOff(ship->ReferReturnOn());
	//舵の自動戻るモードの状態を調べてランプの状態を調整する
	lamps[LAMP_LIST::RETURN_TO_MIDDLE].Draw();
}

void UserInterface::DrawUIUnderShip(bool lock, Coordinate2D<double> coord
	, Camera camera, double radian) {
	if (lock)
		DrawCircle(coord,camera);
	if (!lock)
		DrawMyCircle(camera, radian);
}

void UserInterface::DrawShipOnTheMap(double X, double Z,bool enemy) {
	
	unsigned int enemyCr = GetColor(255, 0, 0);
	unsigned int allyCr = GetColor(0, 0, 255);

	//敵と自分の色が違う
	unsigned int realCr = enemy ? enemyCr : allyCr;

	//実際の位置を計算する
	double realX = abs((X + MapSize::xSize) /
		(MapSize::xSize * 2)) * mapPaperSize * 2;
	double realZ = abs((Z + MapSize::zSize) /
		(MapSize::zSize * 2)) * mapPaperSize * 1.2f;
	
	/*ミニマップに点を描く*/
	DrawBox(
		Screen::SCREEN_X / 2 - (int)mapPaperSize + (int)realX - 3,
		Screen::SCREEN_Z - (int)(((1.2 * (int)mapPaperSize) - realZ)),
		Screen::SCREEN_X / 2 - (int)mapPaperSize + (int)realX + 3,
		Screen::SCREEN_Z - (int)(((1.2 * (int)mapPaperSize) - realZ) - 4),
		realCr,TRUE);
}

/*左上の座標を使って描く*/
void UserInterface::DrawPictureByLeftUp(int SN, double X, 
	double Z, double Multiple) {
	int startX, startZ;
	int graphSizeX, graphSizeZ;

	/*座標、サイズを計算する*/
	GetGraphSize(*handleList[SN], &graphSizeX, &graphSizeZ);
	startX = (int)(Screen::SCREEN_X * X);
	startZ = (int)(Screen::SCREEN_Z * Z);
	/*画像を描く*/
	DrawExtendGraph(
		startX, startZ,
		(int)(startX + graphSizeX * Multiple),
		(int)(startZ + graphSizeZ * Multiple),
		*handleList[SN], TRUE
	);
}

/*中心のＸ座標と上のＺ座標を使って描く*/
void UserInterface::DrawPictureByCenterOnX(int SN, double X,
	double Z, double Multiple) {
	int startX, startZ;
	int graphSizeX, graphSizeZ;

	/*座標、サイズを計算する*/
	GetGraphSize(*handleList[SN], &graphSizeX, &graphSizeZ);
	startX = (int)((Screen::SCREEN_X * X) -
		(graphSizeX * Multiple) * 0.5);
	startZ = (int)(Screen::SCREEN_Z * Z);
	DrawExtendGraph(
		startX, startZ,
		(int)(startX + graphSizeX * Multiple),
		(int)(startZ + graphSizeZ * Multiple),
		*handleList[SN], TRUE
	);
}

/*中心座標を利用して描く*/
void UserInterface::DrawPictureByCenter(int SN, double X,
	double Z, double Multiple) {
	int startX, startZ;
	int graphSizeX, graphSizeZ;

	/*座標、サイズを計算する*/
	GetGraphSize(*handleList[SN], &graphSizeX, &graphSizeZ);
	startX = (int)((Screen::SCREEN_X * X) -
		(graphSizeX * Multiple) * 0.5);
	startZ = (int)((Screen::SCREEN_Z * Z) -
		(graphSizeZ * Multiple) * 0.5);
	DrawExtendGraph(
		startX, startZ,
		(int)(startX + graphSizeX * Multiple),
		(int)(startZ + graphSizeZ * Multiple),
		*handleList[SN], TRUE
	);
}

/*旋回グラフを描く-旋回点は左上*/
void UserInterface::DrawRotatePicture(int SN, double X, double Z, double Multiple,
	double Radian) {
	int startX, startZ;
	int graphSizeX, graphSizeZ;

	startX = (int)(Screen::SCREEN_X * X);
	startZ = (int)(Screen::SCREEN_Z * Z);

	GetGraphSize(*handleList[SN], &graphSizeX, &graphSizeZ);
	DrawRotaGraph3(
		startX,startZ,
		0, 0,
		Multiple,
		Multiple,
		Radian, *handleList[SN], TRUE, FALSE
	);
}

/*旋回グラフを描く-旋回点は中心*/
void UserInterface::DrawRotatePicture2(int SN, double X, double Z, double Multiple,
	double Radian) {
	int graphSizeX, graphSizeZ;

	GetGraphSize(*handleList[SN], &graphSizeX, &graphSizeZ);

	DrawRotaGraph3(
		(int)X, (int)Z,
		graphSizeX / 2, graphSizeZ / 2,
		Multiple,
		Multiple,
		Radian, *handleList[SN], TRUE, FALSE
	);
}

void UserInterface::DrawBoxWithPerAndCr(Coordinate2D<double> LeftUp,
	Coordinate2D<double>Size,
	double Percentage, int Cr) {
	DrawBox((int)LeftUp.x, (int)LeftUp.z, (int)(LeftUp.x + Size.x*Percentage),
		(int)(LeftUp.z + Size.z), Cr, TRUE);
}

/*ロックの円を描く*/
void UserInterface::DrawCircle(Coordinate2D<double> coord, Camera camera) {
	DrawRotatePicture2(UI_LIST::LOCK_CIRCLE, 
		(coord.x - camera.ReferRealCameraX())*camera.ReferZoomRatio(), 
		(coord.z - camera.ReferRealCameraZ())*camera.ReferZoomRatio(),
		USER_INTERFACE_POSITION::LOCK_CIRCLE * camera.ReferZoomRatio(),
		radian_returnTheLockCircle);
	radian_returnTheLockCircle += MathAndPhysics::PI / 720;
}

void UserInterface::DrawHPBar(Coordinate2D<double> coord, int HP, int maxHP) {
	Coordinate2D<double> plus = { 73,8 };//ＣＵＩの画像サイズに合わせる
	DrawBoxWithPerAndCr(coord, plus, 1, CrBox[BLACK]);

	unsigned int tempCr;
	double per;
	per = (double)HP / (double)maxHP;

	if (per > 0.75)
		tempCr = CrBox[MEDIUM_SEA_GREEN];
	else if (per > 0.5)
		tempCr = CrBox[MY_SIN];
	else if (per > 0.25)
		tempCr = CrBox[TENNE];
	else
		tempCr = CrBox[GRENADIER];

	DrawBoxWithPerAndCr(coord, plus, per, tempCr);
}

/*自分のしたにある円を描く*/
void UserInterface::DrawMyCircle(Camera camera, double radian) {
	int xOnScreen;
	int zOnScreen;

	xOnScreen = Screen::SCREEN_X / 2;
	zOnScreen = Screen::SCREEN_Z / 2;

	SetTrans(120);
	DrawRotatePicture2(UI_LIST::MY_DIRECT,
		xOnScreen, zOnScreen, 
		USER_INTERFACE_POSITION::MY_DIRECT_MULTI * camera.ReferZoomRatio(),
		radian);
	ResetTrans();
}

void UserInterface::SetColorBox() {
	CrBox[GREEN] = GetColor(0, 255, 0);
	CrBox[RED] = GetColor(255, 0, 0);
	CrBox[BLUE] = GetColor(0, 0, 255);
	CrBox[BLACK] = GetColor(0, 0, 0);
	CrBox[WHITE] = GetColor(255, 255, 255);
	CrBox[MIDNIGHT] = GetColor(33, 47, 61);
	CrBox[GRENADIER] = GetColor(203, 67, 53);
	CrBox[LOCHMARA] = GetColor(36, 113, 163);
	CrBox[MY_SIN] = GetColor(245, 176, 65);
	CrBox[TENNE] = GetColor(211, 84, 0);
	CrBox[MEDIUM_SEA_GREEN] = GetColor(46, 204, 113);
}