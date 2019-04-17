#include "ShipMain.h"

ShipMain::~ShipMain()
{
}

void ShipMain::InifThisShip(int *ShipHandle, int *SShadowH ,int ShipNum,
	EffectTemplate ET, SoundLoader *SL) {
	SetPictureHandle(ShipHandle);
	SetShadowHandle(SShadowH);
	//GetDataFromShipdata(ShipNum);
	MemorySecure();
	LoadSound(SL);
	FindThosePoint();//あたり判定用ポイントを取得する

	bubblePointCount = AUTO_SHIP_DATA::AROUND_POINT;
	for (int i = 0; i < AUTO_SHIP_DATA::AROUND_POINT; i++) {
		bubbleStartPoint[i] = ET.CopyFromCreateBubble();
		bubbleStartPoint[i].InifCoordinate(aroundPointPos[i].x,
			aroundPointPos[i].z, true, 3400, 20, true,
			0.15, 1.001);
	}//泡生成ポイントを設置

	for (int i = 0; i < 2; i++) {
		smokeStartPoint[i] = ET.CopyFromCreateSmoke();
		smokeStartPoint[i].InifCoordinate(4, 0, true, 3000, 20, true,
			0.06, 1.005);
	}
	smokeStartPoint[0].InifCoordinate(14, 0, true, 2500, 20, true,
		0.06, 1.005);
	smokeStartPoint[1].InifCoordinate(-12, 0, true, 2500, 20, true,
		0.06, 1.005);
}

void ShipMain::FindThosePoint() {
	using namespace AUTO_SHIP_DATA;

	int sx, sz;
	GetGraphSize(*ReferGraphHandle(), &sx, &sz);//画像の大きさを取る
	int hTemp = MakeScreen(sx, sz, TRUE);//空きのグラフィックを作成する
	SetDrawScreen(hTemp);//描画先を指定
	ClearDrawScreen();//画面クリア
	DrawGraph(0, 0, *ReferGraphHandle(), TRUE);//船を描画
	ScreenFlip();
	unsigned int Cr = GetPixel(1, 1);//透過色
	bool flg = false;
	int num = 0;//配列番号


	//初期化
	for (int i = 0; i < AROUND_POINT; i++) {
		aroundPointPos[i].x = aroundPointPos[i].z = 0;
	}
	for (int xx = (int)sx / (AROUND_POINT / 2);
		xx < (int)sx - (int)sx / 
		(AROUND_POINT / 2);
		xx += (int)sx / (AROUND_POINT / 2)) {
		// 上から下へ1ドットずつ調べる
		for (int yy = 0; yy < (int)sz; yy++) {
			if (GetPixel(xx, yy) != Cr) {	// 透過色ではなくなったとき
				aroundPointPos[num].x = xx * ReferMultiple() -
					sx * ReferMultiple() * .5;			// 座標の登録
				aroundPointPos[num].z = yy * ReferMultiple() -
					sz * ReferMultiple() * .5;
				if (num++ >= AROUND_POINT - 1) flg = true;	// 配列をオーバーしたらやめるフラグ
				break;
			}
		}
		if (flg) break;	// 配列をオーバーしたらやめる
		// 下から上へ1ドットずつ調べる
		for (int yy = (int)sz - 1; yy > 0; yy--) {
			if (GetPixel(xx, yy) != Cr) {	// 透過色ではなくなったとき
				aroundPointPos[num].x = xx * ReferMultiple() -
					sx * ReferMultiple() * .5;			// 座標の登録
				aroundPointPos[num].z = yy * ReferMultiple() -
					sz * ReferMultiple() * .5;
				if (num++ >= AROUND_POINT - 1) flg = true;	// 配列をオーバーしたらやめるフラグ
				break;
			}
		}
		if (flg) break;	// 配列をオーバーしたらやめる
	}
	pointCount = num;
}

/*出力パーセンテージ変更*/
void ShipMain::ChangeAccPercentage(bool up) {
	if (up&&currentAccPercentage < 1.0f) {
		currentAccPercentage += 0.25f;
		mainEngine.SetOutPutPercentage(currentAccPercentage);
	}
	if (!up&&currentAccPercentage > -0.25f) {
		currentAccPercentage -= 0.25f;
		mainEngine.SetOutPutPercentage(currentAccPercentage);
	}
}

void ShipMain::CalSpeed() {
	/*物理演算に変わりました*/
	mainEngine.Update();
	SetOutput(mainEngine.ReferOutput());
}

/*進行方向変更*/
void ShipMain::ChangeDirect(bool right) {
	if (right) {
		currentRadian += radianChangePerFrame;
	}
	else {
		currentRadian -= radianChangePerFrame;
	}
	if (currentRadian > maxRadian)
		currentRadian = maxRadian;
	if (currentRadian < -maxRadian)
		currentRadian = -maxRadian;
}

/*舵が自動的に真中に戻るか*/
void ShipMain::Alignment() {

	if (returnToCenter == true) {
		if (currentRadian > MathAndPhysics::PI / 10000)
			currentRadian -= MathAndPhysics::PI / 10000;
		if (currentRadian < -MathAndPhysics::PI / 10000)
			currentRadian += MathAndPhysics::PI / 10000;
		if ((currentRadian < MathAndPhysics::PI / 10000 
			&& currentRadian>0) ||
			(currentRadian > -MathAndPhysics::PI / 10000 
				&& currentRadian < 0)) {
			currentRadian = 0;
		}
		if (currentRadian == 0)
			returnToCenter = false;
	}
	SetRadianChangePerFrame(currentRadian);
}

/*ここの処理はコマンドに関わるものだけです*/
void ShipMain::ControlThisShip(int Command) {;
	switch (Command) {
	case CommandSerial::NONE_COMMAND:break;
	case CommandSerial::INCREASE_OUTPUT:ChangeAccPercentage(true); break;
	case CommandSerial::DECREASE_OUTPUT:ChangeAccPercentage(false); break;
	case CommandSerial::TURN_RIGHT:ChangeDirect(true); break;
	case CommandSerial::TURN_LEFT:ChangeDirect(false); break;
	case CommandSerial::TURN_RETURN:ReturnDirectChange(); break;
	case CommandSerial::TURRET_TURN_RIGHT:TurnMainWeapon(true); break;
	case CommandSerial::TURRET_TURN_LEFT:TurnMainWeapon(false); break;
	case CommandSerial::TURRET_PULLUP:PullMainWeapon(true); break;
	case CommandSerial::TURRET_PULLDOWN:PullMainWeapon(false); break;
	}
}

void ShipMain::Update() {
	CalSpeed();
	Alignment();
	CalThePoint();
}

void ShipMain::TEST() {
	/*ここに入れたものは最後全部shipdataに統合する*/
	SetMass(30000);

	/*エンジン状態の初期化*/
	mainEngine.Inif(30, 0.75);
	SetMaxOutput(mainEngine.ReferMaxOutput());
	SetMaxSpeedOnZ(0.8);

	/*移動関連*/
	SetSpeed(0);
	maxRadian = MathAndPhysics::PI / 10;
	radianChangePerFrame = MathAndPhysics::PI / 1800;
	currentRadian = 0;

	serialNumber = 1;
	shipCrashR = 200;

	/*エフェクト関連*/
	smokePointCount = 2;

	/*音声関連*/
}

/*デバッグ用テスト関数*/
void ShipMain::TestDraw(double x, double z) {
	for (int i = 0; i < bubblePointCount; i++) {
		bubbleStartPoint[i].TestDraw(ReferRadianOnZ(),ReferCoordX(),
			ReferCoordZ(),x,z);
	}
	for (int i = 0; i < 2; i++) {
		smokeStartPoint[i].TestDraw(ReferRadianOnZ(), ReferCoordX(),
			ReferCoordZ(),x,z);
	}
}

void ShipMain::MemorySecure() {
	MainParts = new ShipCrashParts [10];
	SubParts = new ShipCrashParts[10];

	MainWeapon = new Weapon[10];
	SubWeapon = new Weapon[10];

	bubbleStartPoint = new EffectPoint[AUTO_SHIP_DATA::AROUND_POINT];
	smokeStartPoint = new EffectPoint[2];
}

/*削除前に必ずこの関数を呼び出して！*/
void ShipMain::DestroyMemory() {
	delete[] MainParts;
	delete[] SubParts;
	delete[] MainWeapon;
	delete[] SubWeapon;
	delete[] bubbleStartPoint;
	delete[] smokeStartPoint;
}

//エフェクト生成
Effect ShipMain::NewBubble(int num) {
	double newRadian = ReferRadianOnZ() - MathAndPhysics::PI;

	return bubbleStartPoint[num].NewEffect(newRadian,
		ReferSpeedOnZ(),
		ReferCoordX(), ReferCoordZ());
}

Effect ShipMain::NewSmoke(int num) {
	double newRadian = ReferRadianOnZ() - MathAndPhysics::PI;

	//大体の方向が変わらないが、少しだけ変わるようにする
	newRadian += (double)((1 - rand() % 3) / 180.0f) * MathAndPhysics::PI;

	//生成したエフェクトを初期化して返す
	return smokeStartPoint[num].NewEffect(newRadian, ReferSpeedOnZ(),
		ReferCoordX(), ReferCoordZ());
}

/*音声関連*/
void ShipMain::LoadSound(SoundLoader *SL) {
	soundEngine = SL->referShipSoundEngine();
	soundMoving = SL->referShipSoundMoving();
	soundSlow = SL->referShipSoundSlow();
	soundTurning = SL->referShipSoundTuring();

	soundHordHigh = SL->referShipHordLargeHigh();
	soundHordLow = SL->referShipHordLargeLow();
}

void ShipMain::CheckAndPlaySound() {
	/*エンジン音*/
	if (currentAccPercentage != 0 && CheckSoundMem(*soundEngine) == 0) {
		PlaySoundMem(*soundEngine, DX_PLAYTYPE_LOOP, TRUE);
	}
	if (currentAccPercentage == 0 && CheckSoundMem(*soundEngine) != 0) {
		StopSoundMem(*soundEngine);
	}
	/*水音*/
	if (ReferSpeedOnZ() > 0 && ReferSpeedOnZ() < 0.2&&
		CheckSoundMem(*soundSlow) == 0) {
		PlaySoundMem(*soundSlow,DX_PLAYTYPE_LOOP,TRUE);
	}
	if ((ReferSpeedOnZ() < 0 || ReferSpeedOnZ() >= 0.2)&&
		CheckSoundMem(*soundSlow) != 0) {
		StopSoundMem(*soundSlow);
	}

	if (ReferSpeedOnZ() >= 0.2 && CheckSoundMem(*soundMoving) == 0) {
		PlaySoundMem(*soundMoving, DX_PLAYTYPE_LOOP, TRUE);
	}
	if (ReferSpeedOnZ() < 0.2 && CheckSoundMem(*soundMoving) != 0) {
		StopSoundMem(*soundMoving);
	}

	/*転向音*/
	if (abs(currentRadian) > MathAndPhysics::PI / 30 &&
		CheckSoundMem(*soundTurning) == 0) {
		PlaySoundMem(*soundTurning, DX_PLAYTYPE_LOOP, TRUE);
	}
	if (abs(currentRadian) < MathAndPhysics::PI / 30 &&
		CheckSoundMem(*soundTurning) != 0) {
		StopSoundMem(*soundTurning);
	}
}

/*武器の使える状態を確認*/
bool ShipMain::IsThisOneUsable(int Num, bool Main) {
	if (Main)
		return MainWeapon[Num].ReferShootable();
	else
		return SubWeapon[Num].ReferShootable();
}

Ammo ShipMain::Shoot(int Num, bool Main) {
	if (Main)
		return MainWeapon[Num].Shoot(ReferCoordX(),ReferCoordZ(),ReferRadianOnZ());
	else
		return SubWeapon[Num].Shoot(ReferCoordX(),ReferCoordZ(), ReferRadianOnZ());
}

void ShipMain::SetWeaponTest(PictureLoader *PL) {
	MainWeaponCount = 8;
	
	for (int i = 0; i < MainWeaponCount; i++) {
		Weapon Weapon(20.0, 8.0 - i * 2, 10.0, 10.0,
			0.0, 0.2, 100,
			50, 12, PL->ReferAmmoHandle(0), 17, 20,serialNumber);
		MainWeapon[i] = Weapon;
		MainWeapon[i].SetCoolDownTime(3200);//3200
	}
	fireControllerMain.SetSpeed(MainWeapon[0].ReferInitialSpeed());
	//速度を読み込み、初期化する
	fireControllerMain.InifDistance(MainWeapon[0].ReferMaxRadianOnY(), 0);
}

void ShipMain::ShowMePointOfImpact(Camera camera) {
	DrawMainPoint(camera);
}

//メイン武器の角度を変更
//returnはtrueの時は制限範囲に至る、なければfalseを返す
bool ShipMain::TurnMainWeapon(bool right) {
	for (int i = 0; i < MainWeaponCount; i++) {
		MainWeapon[i].Turn(right);
	}
	return false;
}

bool ShipMain::PullMainWeapon(bool up) {
	bool end;
	for (int i = 0; i < MainWeaponCount; i++) {
		end = MainWeapon[i].Pull(up);
	}
	return end;
}

void ShipMain::CalThePoint() {
	CalMainPoint();
}

void ShipMain::CalMainPoint() {
	/*射撃データは計算部分に使われるため、本番の計算は最後にすう*/
	fireControllerMain.SetCoord(MainWeapon[0].ReferRealCoord(ReferCoord(),
		ReferRadianOnZ()));//船の座標を設定
	RadianNeededIn3D temp = MainWeapon[0].ReferRadian();
	temp.z += ReferRadianOnZ();//本番の角度に設定する
	fireControllerMain.SetRadian(temp);//今砲塔の角度を設定
	fireControllerMain.CalTheAnswer();//目標座標を計算する
}


void ShipMain::DrawMainPoint(Camera camera) {
	Coordinate<double> point = fireControllerMain.ReferAnswer();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 170);//透明度を下がる
	DrawCircleAA((float)(point.x - camera.ReferRealCameraX()),
		(float)(point.z-camera.ReferRealCameraZ()),
		15,16,GetColor(255,0,0),0,0.5);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);//透明度を下がる
}

/*戦闘部分*/
void ShipMain::SufferDamage(int damage) {
	hitPoint -= damage;
	if (hitPoint <= 0)
		Killed();
}

/*ロック管理*/
void ShipMain::TestLock(ShipMain *ship) {
	CalDistance(ship);//距離を計算する

	double x1, x2, z1, z2;
	x1 = ship->ReferCoordX();
	x2 = ReferCoordX();
	z1 = ship->ReferCoordZ();
	z2 = ReferCoordZ();

	//逆三角関数を利用してラジアンを計算する
	targetRadianOnZforMain = atan((z2 - z1) / (x2 - x1));

	if (x1 > x2 && z1 < z2) {//敵は第一象限にいる
		targetRadianOnZforMain = targetRadianOnZforMain;
	}
	else if (x1 > x2 && z1 > z2) {//敵は第二象限にいる
		targetRadianOnZforMain = -targetRadianOnZforMain;
	}
	else if (x1 < x2 && z1 > z2) {//敵は第三象限にいる
		targetRadianOnZforMain = targetRadianOnZforMain + MathAndPhysics::PI;
	}
	else {//敵は第四象限にいる
		targetRadianOnZforMain = MathAndPhysics::PI - targetRadianOnZforMain;
	}

	//targetRadianOnZforMain = asin((z2 - z1) / sqrt((pow(z2 - z1, 2) + pow(x2 - x1, 2))));

	/*垂直角度の計算*/
	targetRadianForMain = fireControllerMain.CalDistanceAndTellMeRadianOnY(distance);

	/*メイン武器の調整*/
	/*垂直*/
	if (MainWeapon[0].ReferRadianOnY() - targetRadianForMain > 0.03) {
		PullMainWeapon(false);
	}
	else if (MainWeapon->ReferRadianOnY() - targetRadianForMain < 0.03) {
		PullMainWeapon(true);
	}
	/*水平*/
	if (MainWeapon[0].ReferRadianOnZ() + ReferRadianOnZ() -
		targetRadianOnZforMain > 0.03) {
		TurnMainWeapon(false);
	}
	else if (MainWeapon[0].ReferRadianOnZ() + ReferRadianOnZ() -
		targetRadianOnZforMain < 0.03) {
		TurnMainWeapon(true);
	}
}

void ShipMain::CalDistance(ShipMain *ship) {
	distance = Distance2D(ship->ReferCoord(), ReferCoord());
}