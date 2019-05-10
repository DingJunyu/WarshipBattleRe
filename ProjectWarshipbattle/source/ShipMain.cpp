#include "ShipMain.h"

ShipMain::~ShipMain()
{
}

void ShipMain::InifThisShip(int *ShipHandle, int *SShadowH, int ShipNum,
	EffectTemplate ET, SoundLoader *SL) {
	SetPictureHandle(ShipHandle);
	SetShadowHandle(SShadowH);
	//GetDataFromShipdata(ShipNum);
	thisShipType = ShipNum;
	MemorySecure();//メモリ確保
	LoadSound(SL);//音を読み込む
	FindThosePoint();//あたり判定用ポイントを取得する

	/*あたり判定用ポイントを利用して泡生成ポイントを作る*/
	bubblePointCount = AUTO_SHIP_DATA::AROUND_POINT;
	for (int i = 0; i < AUTO_SHIP_DATA::AROUND_POINT; i++) {
		bubbleStartPoint[i] = ET.CopyFromCreateBubble();
		bubbleStartPoint[i].InifCoordinate(aroundPointPos[i].x,
			aroundPointPos[i].z, true, 3400, 20, true,
			0.15, 1.001);
	}//泡生成ポイントを設置

	for (int i = 0; i < 2; i++) {
		smokeStartPoint[i] = ET.CopyFromCreateSmoke();//テンプレートからコピーする
		/*初期化する*/
		smokeStartPoint[i].InifCoordinate(4, 0, true, 3000, 20, true,
			0.06, 1.005);
	}
	smokeStartPoint[0].InifCoordinate(14, 0, true, 2500, 20, true,
		0.06, 1.005);
	smokeStartPoint[1].InifCoordinate(-12, 0, true, 2500, 20, true,
		0.06, 1.005);
}

/*ファイル読み込みが失敗したらfalseを戻る*/
bool ShipMain::InifThisShip(PictureLoader *PL, EffectTemplate ET,
	SoundLoader *SL, int num,int SN) {

	int count = 0;

	char readData[50][50];
	char trash[50];
	double doubleData[50];

	serialNumber = SN;

	/*ファイル操作*/
	FILE *filePointer;//ファイルポインター
	char fileName[100];
	sprintf_s(fileName, 100, ".//Data//ShipNumList//%d.csv", num);

	if ((fopen_s(&filePointer, fileName, "r")) != 0) {
		return false;
	}

	/*読み込む*/
	while (1) {
		fscanf_s(filePointer,"%[^,]", &readData[count], 20);//数字を読む
		doubleData[count] = atof(readData[count]);
		fscanf_s(filePointer,"%*[,]%s", &trash, 20);

		count++;
		if (feof(filePointer))/*読み込み中止*/
			break;
	}

	fclose(filePointer);//ファイルを閉じる

	SetPictureHandle(PL->ReferShipHandle(num));
	SetShadowHandle(PL->ReferShipShadowHandle(num));
	SetLength(PL->ReferShipSizeX());
	SetWidth(PL->ReferShipSizeZ());
	thisShipType = num;
	MemorySecure();//メモリ確保
	LoadSound(SL);//音を読み込む
	FindThosePoint();//あたり判定用ポイントを取得する

	/*あたり判定用ポイントを利用して泡生成ポイントを作る*/
	bubblePointCount = AUTO_SHIP_DATA::AROUND_POINT;
	for (int i = 0; i < AUTO_SHIP_DATA::AROUND_POINT; i++) {
		bubbleStartPoint[i] = ET.CopyFromCreateBubble();
		bubbleStartPoint[i].InifCoordinate(aroundPointPos[i].x,
			aroundPointPos[i].z, true, 3400, 20, true,
			0.15, 1.001);
	}//泡生成ポイントを設置

	int pointer = 0;//データを読み込む用ポインター
					
	SetMultiple(doubleData[pointer]);
	pointer++;
	//ＨＰを設置する
	maxHitPoint = hitPoint = (int)doubleData[pointer];
	pointer++;

	//スモーク生成点を設置する
	smokePointCount  = (int)doubleData[pointer];
	pointer++;

	int smokeTime;//スモークの継続時間を設置
	smokeTime = (int)doubleData[pointer];
	pointer++;

	for (int i = 0; i < smokePointCount; i++) {
		smokeStartPoint[i] = ET.CopyFromCreateSmoke();//テンプレートからコピーする
		/*初期化する*/
		smokeStartPoint[i].InifCoordinate(doubleData[pointer],
			doubleData[pointer + 1], true, smokeTime, 20, true,
			0.06, 1.005);
		pointer += 2;
	}

	/*注意:設置した後にポインターはもう次の行に指しているから＋＋をしないで*/
	/*あたり判定用ボックスを設置する*/
	shipMainCrash = { doubleData[pointer],doubleData[pointer + 1], 
		doubleData[pointer + 2]};
	pointer += 3;

	/*重力を設置する*/
	SetMass(doubleData[pointer]);
	pointer++;
	/*エンジンの出力を設置する*/
	mainEngine.Inif(doubleData[pointer], doubleData[pointer + 1]);
	pointer += 2;
	SetMaxOutput(mainEngine.ReferMaxOutput());//最大出力をAllMovableObjectに設定する
	/*最大速度を設置する*/
	SetMaxSpeedOnZ(doubleData[pointer]);
	pointer++;

	SetSpeed(0);//今の速度を０に設定する
	maxRadian = MathAndPhysics::PI / 6;//最大角度を18度に設定する
	radianChangePerFrame = MathAndPhysics::PI / 900;//舵旋回速度を設置
	currentRadian = 0;//舵を原点に設定する

	/*武器通用ステータスを記録*/
	int weaponCD = (int)doubleData[pointer];
	pointer++;
	int weaponDamage = (int)doubleData[pointer];
	pointer++;
	double weaponHigh = doubleData[pointer];
	pointer++;
	double weaponInitialSpeed = doubleData[pointer];
	pointer++;

	MainWeaponCount = (int)doubleData[pointer];
	pointer++;

	for (int i = 0; i < MainWeaponCount; i++) {
		Weapon Weapon(doubleData[pointer], doubleData[pointer + 1],
			10.0, 10.0,//長さと横の長さ-まだ使っていません
			0.0, 0.2, 100,//初期角度、ＨＰ（まだ使っていません）
			50, 12, //装甲値（まだ使っていません）
			PL->ReferAmmoHandle(0), weaponInitialSpeed, weaponHigh,
			serialNumber);
		MainWeapon[i] = Weapon;
		MainWeapon[i].SetCoolDownTime(weaponCD);//射撃間隔を設定する
		MainWeapon[i].SetDamage(weaponDamage);
		pointer += 2;
	}
	/*武器のステータスを射撃コントロールにあげる*/
	fireControllerMain.SetSpeed(MainWeapon[0].ReferInitialSpeed());
	//速度を読み込み、初期化する
	fireControllerMain.InifDistance(MainWeapon[0].ReferMaxRadianOnY(), 0);
	
	return true;
}

/*ファイル操作を入れたのため、初期化の速度が大幅早くなりました*/
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

	FILE *filePointer;//ファイルポインター
	char fileName[100];
	sprintf_s(fileName, 100, ".//Data//Points//%d.txt", thisShipType);

	/*もし保存されたデータがある場合はそのまま読み込む*/
	if ((fopen_s(&filePointer, fileName, "r")) == 0) {
		int j;
		fscanf_s(filePointer, "%d\n", &j);
		for (int i = 0; i < j; i++) {
			fscanf_s(filePointer, "%lf %lf\n", &aroundPointPos[i].x, &aroundPointPos[i].z);
			/*ここはエラー処理が必要だと思います*/
		}
		
		pointCount = AROUND_POINT;
		fclose(filePointer);
		return;//読み込んだら関数を終了
	}

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

	/*もしファイルがいなかった場合は新たにファイルを作って、データを保存する*/
	fopen_s(&filePointer, fileName, "w");
	fprintf(filePointer, "%d\n", num);
	for (int i = 0; i < AROUND_POINT; i++) {
		fprintf(filePointer, "%lf %lf\n", aroundPointPos[i].x, aroundPointPos[i].z);
		//座標を記録する
	}
	fclose(filePointer);//ファイルを閉じる

	pointCount = num;//数を記録する
}

/*出力パーセンテージ変更*/
void ShipMain::SetEngineOutPutRate(bool up) {
	if (up&&currentEngineOutPutRate < 1.0f) {//範囲内であれば
		currentEngineOutPutRate += 0.25f;	//増加する
	}
	if (!up&&currentEngineOutPutRate > -0.25f) {//範囲内であれば
		currentEngineOutPutRate -= 0.25f; //減少する
	}
	mainEngine.SetOutPutPercentage(currentEngineOutPutRate);//今の出力を与える
}

void ShipMain::CalSpeed() {
	/*物理演算に変わりました*/
	mainEngine.Update();//エンジンのステータスを更新
	SetOutput(mainEngine.ReferOutput());//更新された出力を設定する
}

/*進行方向変更*/
void ShipMain::ChangeDirect(bool right) {
	if (right) {
		currentRadian += radianChangePerFrame;//右旋回
	}
	else {
		currentRadian -= radianChangePerFrame;//左旋回
	}
	if (currentRadian > maxRadian)//右の範囲検査
		currentRadian = maxRadian;
	if (currentRadian < -maxRadian)//左の範囲検査
		currentRadian = -maxRadian;
}

/*舵が自動的に真中に戻るか*/
void ShipMain::Alignment() {

	if (returnToCenter == true) {
		//舵が右にある時
		if (currentRadian > MathAndPhysics::PI / 2000)
			currentRadian -= MathAndPhysics::PI / 2000;
		//舵が左にある時
		if (currentRadian < -MathAndPhysics::PI / 2000)
			currentRadian += MathAndPhysics::PI / 2000;
		/*中心に近くなると中心に置くようにする*/
		if ((currentRadian < MathAndPhysics::PI / 2000
			&& currentRadian>0) ||
			(currentRadian > -MathAndPhysics::PI / 2000 
				&& currentRadian < 0)) {
			currentRadian = 0;
		}
		/*舵が中心にある時に自動戻る状態を解除*/
		if (currentRadian == 0)
			returnToCenter = false;
	}
	SetRadianChangePerFrame(currentRadian);//今の状態を更新する
}

/*ここの処理はコマンドに関わるものだけです*/
void ShipMain::ControlThisShip(int Command) {;
	switch (Command) {
	case CommandSerial::NONE_COMMAND:break;
	case CommandSerial::INCREASE_OUTPUT:SetEngineOutPutRate(true); break;
	case CommandSerial::DECREASE_OUTPUT:SetEngineOutPutRate(false); break;
	case CommandSerial::TURN_RIGHT:ChangeDirect(true); break;
	case CommandSerial::TURN_LEFT:ChangeDirect(false); break;
	case CommandSerial::TURN_RETURN:ReturnDirectChange(); break;
	case CommandSerial::TURRET_TURN_RIGHT:TurnMainWeapon(true); break;
	case CommandSerial::TURRET_TURN_LEFT:TurnMainWeapon(false); break;
	case CommandSerial::TURRET_PULLUP:PullMainWeapon(true); break;
	case CommandSerial::TURRET_PULLDOWN:PullMainWeapon(false); break;
	case CommandSerial::FORECAST_PLUS:ChangeForecastSecond(true); break;
	case CommandSerial::FORECAST_MINUS:ChangeForecastSecond(false); break;
	case CommandSerial::REVISE_RADIAN_ON_Z_PLUS:ChangeReviseRadianOnY(true); break;
	case CommandSerial::REVISE_RADIAN_ON_Z_MINUS:ChangeReviseRadianOnY(false); break;
	}
}

/*フレームごとに更新する*/
void ShipMain::Update() {
	CalSpeed();//出力を計算する
	Alignment();//角度を計算する
	CalThePoint();//砲弾落下地点を計算する
}

void ShipMain::TEST() {
	/*ここに入れたものは最後全部shipdataに統合する*/
	SetMass(30000);

	/*エンジン状態の初期化*/
	mainEngine.Inif(30, 0.75);
	SetMaxOutput(mainEngine.ReferMaxOutput());//最大出力をAllMovableObjectに設定する
	SetMaxSpeedOnZ(0.8);//最大速度を設定する

	/*移動関連*/
	SetSpeed(0);//今の速度を０に設定する
	maxRadian = MathAndPhysics::PI / 6;//最大角度を18度に設定する
	radianChangePerFrame = MathAndPhysics::PI / 900;//舵旋回速度を設置
	currentRadian = 0;//舵を原点に設定する

	serialNumber = 1;//番号
	shipCrashR = 200;

	/*エフェクト関連*/
	smokePointCount = 2;

	/*音声関連*/
}

/*デバッグ用テスト関数*/
void ShipMain::TestDraw(double x, double z) {
	//生成ポイントを描画する
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
	MainParts = new ShipCrashParts[10];
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
	/*船と正反対の方向に向く*/
	double newRadian = ReferRadianOnZ() - MathAndPhysics::PI;

	//バブルポイントから泡を生成する
	return bubbleStartPoint[num].NewEffect(newRadian,
		ReferSpeedOnZ(),
		ReferCoordX(), ReferCoordZ());
}

Effect ShipMain::NewSmoke(int num) {
	/*船と正反対の方向に向く*/
	double newRadian = ReferRadianOnZ() - MathAndPhysics::PI;

	//大体の方向が変わらないが、少しだけ変わるようにする
	newRadian += (double)((1 - rand() % 3) / 180.0f) * MathAndPhysics::PI;

	//生成したエフェクトを初期化して返す
	return smokeStartPoint[num].NewEffect(newRadian, ReferSpeedOnZ(),
		ReferCoordX(), ReferCoordZ());
}

/*音声関連*/
void ShipMain::LoadSound(SoundLoader *SL) {
	/*各部分の音をロードする*/
	soundEngine = SL->referShipSoundEngine();
	soundMoving = SL->referShipSoundMoving();
	soundSlow = SL->referShipSoundSlow();
	soundTurning = SL->referShipSoundTuring();

	soundHordHigh = SL->referShipHordLargeHigh();
	soundHordLow = SL->referShipHordLargeLow();
}

void ShipMain::CheckAndPlaySound() {
	/*エンジン音*/
	if (currentEngineOutPutRate != 0 && CheckSoundMem(*soundEngine) == 0) {
		PlaySoundMem(*soundEngine, DX_PLAYTYPE_LOOP, TRUE);
	}
	if (currentEngineOutPutRate == 0 && CheckSoundMem(*soundEngine) != 0) {
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
	if (Main)//メイン武器
		return MainWeapon[Num].ReferShootable();
	else//サブ武器
		return SubWeapon[Num].ReferShootable();
}

Ammo ShipMain::Shoot(int Num, bool Main) {
	if (Main)//メイン武器
		return MainWeapon[Num].Shoot(ReferCoordX(),ReferCoordZ(),ReferRadianOnZ());
	else//サブ武器
		return SubWeapon[Num].Shoot(ReferCoordX(),ReferCoordZ(), ReferRadianOnZ());
}

void ShipMain::SetWeaponTest(PictureLoader *PL) {
	MainWeaponCount = 8;//数を設定する
	
	for (int i = 0; i < MainWeaponCount; i++) {
		/*武器を初期化する*/
		Weapon Weapon(20.0, 8.0 - i * 2, 10.0, 10.0,
			0.0, 0.2, 100,
			50, 12, PL->ReferAmmoHandle(0), 17, 20,serialNumber);
		MainWeapon[i] = Weapon;
		MainWeapon[i].SetCoolDownTime(3200);//射撃間隔を設定する
		MainWeapon[i].SetDamage(3);
	}
	/*武器のステータスを射撃コントロールにあげる*/
	fireControllerMain.SetSpeed(MainWeapon[0].ReferInitialSpeed());
	//速度を読み込み、初期化する
	fireControllerMain.InifDistance(MainWeapon[0].ReferMaxRadianOnY(), 0);
}

void ShipMain::ShowMePointOfImpact(Camera camera) {
	DrawMainPoint(camera);//落下地点を描く
}

//メイン武器の角度を変更
//入力値：右へ旋回しているか
//returnはtrueの時は制限範囲に至る、なければfalseを返す
bool ShipMain::TurnMainWeapon(bool right) {
	for (int i = 0; i < MainWeaponCount; i++) {
		MainWeapon[i].Turn(right);
	}
	return false;
}

//メイン武器の角度を変更
//入力値：上がっているか
//returnはtrueの時は制限範囲に至る、なければfalseを返す
bool ShipMain::PullMainWeapon(bool up) {
	bool end;
	for (int i = 0; i < MainWeaponCount; i++) {
		end = MainWeapon[i].Pull(up);
	}
	return end;
}

/*落下地点を計算する*/
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

/*砲弾落下地点を○で表示する*/
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
/*ロックオン時の操作*/
void ShipMain::ChangeForecastSecond(bool up) {
	if (up && forecastSeconds <= maxForecastSecond)
		forecastSeconds++;
	if (!up&&forecastSeconds >= -maxForecastSecond)
		forecastSeconds--;
}

/*ロック管理*/
void ShipMain::TestLock(ShipMain *ship, bool render) {

	if (forecastSeconds == 0)
		CalDistance(ship);//距離を計算する
	else
		CalNextPos(ship);


	//目標ラジアン
	double differenceOnY;
	double differenceOnZ;

	canIShoot = false;

	//目標ラジアンを計算する
	differenceOnZ = CalRadianBetweenPoints(
		(forecastSeconds == 0) ? ship->ReferCoord2D_d() : nextPos,
		//予測を使う時は予測地点を使う、使わない時は敵の座標を使う
		ReferCoord2D_d(),
		MainWeapon[0].ReferRadianOnZ() + ReferRadianOnZ());

	/*垂直角度の計算・修正*/
	targetRadianForMain = fireControllerMain.CalDistanceAndTellMeRadianOnY(distance)
		+ fireDataFigureUp.Refercorrection().y;

	differenceOnY = MainWeapon[0].ReferRadianOnY() + reviseRadianOnZ
		- targetRadianForMain;

	/*メイン武器の調整*/
	/*垂直*/
	if (differenceOnY > 0.03) {
		PullMainWeapon(false);
	}
	else if (differenceOnY < 0.03) {
		PullMainWeapon(true);
	}

	/*左へ回す時*/
	if (differenceOnZ > 0.03) {
		TurnMainWeapon(false);
	}
	/*右へ回す時*/
	else if (differenceOnZ < 0.03) {
		TurnMainWeapon(true);
	}

	if (abs(differenceOnY) < 0.06 && abs(differenceOnZ) < 0.1)
		canIShoot = true;
}

void ShipMain::CalDistance(ShipMain *ship) {
	distance = Distance2D(ship->ReferCoord(), ReferCoord());
}

void ShipMain::CalNextPos(ShipMain *ship) {
	nextPos = ship->ReferCoord2D_d();
	NextPoint(&nextPos,ship->ReferRadianOnZ(), ship->ReferSpeedOnZ(),
		forecastSeconds*framesCountInaSecond);
	distance = Distance2D(nextPos, ReferCoord2D_d());
}

void ShipMain::ChangeReviseRadianOnY(bool up) {
	if (reviseRadianOnZ <= maxReviseRadianOnZ && !up)
		reviseRadianOnZ += MathAndPhysics::PI * MathAndPhysics::OneDegree * 0.25;
	if (reviseRadianOnZ >= -maxReviseRadianOnZ && up)
		reviseRadianOnZ -= MathAndPhysics::PI * MathAndPhysics::OneDegree * 0.25;
}