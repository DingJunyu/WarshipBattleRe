#include "IngameDataManagement.h"
/*イテレータを使う前に必ず容器の状況を確認します！！！！！*/
using namespace DxLib;
IngameDataManagement::~IngameDataManagement()
{
}

/*メインルート*/
void IngameDataManagement::Update() {
	SetDrawScreen(DX_SCREEN_BACK);//裏画面に描画する
	ClearDrawScreen();

	AIUpdate();
	LockAndRefresh();
	if (alliesFleet[0].fireDataFigureUp.ReferLockOn() && showLock == true)
		MainCamera.GetXZ(ReferTargetX(alliesFleet[0].fireDataFigureUp.ReferTarget()),
			ReferTargetZ(alliesFleet[0].fireDataFigureUp.ReferTarget()));
	else
		MainCamera.GetXZ(ReferPlayerX(), ReferPlayerZ());//カメラ座標を更新
		
	Control();//コマンドを受け取って、船の状態を変更する
	GetNewEffect();//エフェクトを生成する
	MoveAll();//移動、状態更新


	CheckTeamStatus();//各艦隊の状態を確認し、一方が殲滅されたらゲーム終了

	DeleteUseless();//入らないものを消す


	CheckAndPlaySound();
	DrawAll();//全部更新した後画面を描く
	FC.Wait();
}

/*ゲームコントロール*/
/*保存方式が変わったのため、後直す*/
bool IngameDataManagement::TeamDestroyed() {
	if (alliesFleet.empty() || enemyFleet.empty())
		return false;
	return false;
}

/****************************************************/
/*                      AI管理                      */
/****************************************************/
void IngameDataManagement::AIUpdate() {
	LetFlagShipMove();//敵のフラグシープを動かす
	LetEveryOneMove();
	LetEveryOneLockOn();
	LetEveryOneShoot();
}

void IngameDataManagement::LetFlagShipMove() {
	flagShipAI.LetUsGo(&enemyFleet[0], &alliesFleet[0]);
	enemyFleet[0].SetChangingDirect(flagShipAI.ReferRadianNeededNow());
	enemyFleet[0].SetEngineOutPutRate(flagShipAI.ReferSpeedInNeed());
}

void IngameDataManagement::LetEveryOneMove() {
	ControlThisListMove(&alliesFleet, &AI);
	ControlThisListMove(&enemyFleet, &AI);
}

void IngameDataManagement::ControlThisListMove(std::vector<ShipMain> *shipList,
	ArtificialIntelligence *AI) {	
	int num = 0;
	if (!shipList->empty() && shipList->begin()!=shipList->end()) {
		for (auto ship = shipList->begin();
			ship != shipList->end();
			ship++) {
			if (num == 0) {
				num++;
				continue;
			}
			ship--;
			auto prevShip = ship;
			ship++;
			AI->Move(*ship, *prevShip);
			ship->SetChangingDirect(AI->ReferRadianNeededNow());//変更角度を設置する
			ship->SetEngineOutPutRate(AI->ReferOutPutRateNeededNow());//速度を設置する
		}
	}
}

void IngameDataManagement::LetEveryOneLockOn() {
	ControlThisListLock(&alliesFleet, enemyFleet);
	ControlThisListLock(&enemyFleet, alliesFleet);
}

void IngameDataManagement::ControlThisListLock(std::vector<ShipMain> *shipList,
	std::vector<ShipMain> enemyList) {
	for (auto ship = shipList->begin();
		ship != shipList->end();
		ship++) {
		if (ship->ReferAlive() && !ship->ReferControled()) {
			AI.InBattle(&*ship, enemyList, -1);//ロックターゲットを設置する
			ship->fireDataFigureUp.LockOn_Switch();
		}
	}
}

void IngameDataManagement::LetEveryOneShoot() {
	ControlThisListShoot(&alliesFleet);
	ControlThisListShoot(&enemyFleet);
}

void IngameDataManagement::ControlThisListShoot(std::vector<ShipMain> *shipList) {
	for (auto ship = shipList->begin();
		ship != shipList->end();
		ship++) {
		if (ship->ReferAlive() && //生きている
			!ship->ReferControled() &&//プレーヤーの船じゃない
			ship->ReferCanIShoot()) {//射撃範囲内にいる
			TestShoot(&*ship,false);//射撃
		}
	}
}

/****************************************************/
/*                     描画関連                     */
/****************************************************/
void IngameDataManagement::DrawAll() {
	auto ship = alliesFleet.begin();//イテレータで操作している船のステータスを取る

	/*海を描く*/
	DrawSea();

	//船の下のエフェクトを描画する
	DrawEffectUnderShips();
	/*船のしたにあるUIを描画する*/
	UI.DrawUIUnderShip(ship->fireDataFigureUp.ReferLockOn(),
		enemyFleet[ship->fireDataFigureUp.ReferTarget()].ReferCoord2D_d(),
		MainCamera, ship->ReferRadianOnZ() + ship->ReferChangingRadian());
//	DrawTorpedo();
	DrawShips();//船を描画する
	DrawEffectBeyondShips();//船の上のエフェクトを描画する
//	DrawPlanes();
	DrawAmmo();//弾を描画する
//	DrawBomb();

	DrawPointOfImpact();//砲弾落下予測地点描画

	/*UI関連*/
	UI.DrawUI();//普通のUIを描画する
	UI.DrawUINeedInput(&*ship);//船のステータスに関係があるUIを描画する
	DrawShipsOnMiniMap();//ミニマップを描く
	/*クリック可能なUI*/
	CUI.Draw();//ボタンを描く
	CUI.DrawNeedInput(ship->fireDataFigureUp.ReferLockOn(), 
		ship->fireDataFigureUp.ReferTarget());//引数が必要な部分を描く

	/*テストインフォメーション*/
	if (TEST_SHOW_ON)
		TEST_DRAW();
	
	DxLib::ScreenFlip();
}

/*本体と影を分けて描画する*/
void IngameDataManagement::DrawShips() {
	//友軍を描画する 
	//コントロールしている船が友軍艦隊の一番にする
	if (!alliesFleet.empty())//リスト状況を確認
		for (auto mark = alliesFleet.begin();
			mark != alliesFleet.end(); mark++) {
		if (mark->ReferAlive()) {//ターゲットの状態を確認
//			if (mark == alliesFleet.begin())
//				mark->Draw(MainCamera);//自分だけが画面中心に映す
//			else
				mark->DrawSub(MainCamera);//それ以外のは相対座標を利用して描く
		}
	}
	if(!enemyFleet.empty())//リスト状況を確認
		for (auto ship = enemyFleet.begin();
			ship != enemyFleet.end(); ship++) {
		if (ship->ReferAlive()) {//ターゲットの状態を確認
			ship->DrawSub(MainCamera);//敵軍は全部相対座標を利用して描く
		}
	}
}

void IngameDataManagement::DrawShipsShadow() {
	//友軍を描画する 
	//コントロールしている船が友軍艦隊の一番にする
	if (!alliesFleet.empty())//リスト状況を確認
		for (auto mark = alliesFleet.begin();
			mark != alliesFleet.end(); mark++) {
		if (mark->ReferAlive()) {//ターゲットの状態を確認
//			if (mark == alliesFleet.begin())
//				mark->DrawShadow(MainCamera);//自分だけが画面中心に映す
//			else
				mark->DrawSubShadow(MainCamera);//それ以外のは相対座標を利用して描く
		}
	}
	if (!enemyFleet.empty())//リスト状況を確認
		for (auto ship = enemyFleet.begin();
			ship != enemyFleet.end(); ship++) {
		if (ship->ReferAlive()) {//ターゲットの状態を確認
			ship->DrawSubShadow(MainCamera);//敵軍は全部相対座標を利用して描く
		}
	}
}

void IngameDataManagement::DrawShipsOnMiniMap() {
	//友軍艦隊
	if(!alliesFleet.empty())//リスト状況を確認
	for (auto ship = alliesFleet.begin();
		ship != alliesFleet.end(); ship++) {
		if (ship->ReferAlive()) {//ターゲットの状態を確認
			UI.DrawShipOnTheMap(ship->ReferCoordX(),
				ship->ReferCoordZ(), false);//青いマーク
		}
	}
	if (!enemyFleet.empty())//リスト状況を確認
		for (auto ship = enemyFleet.begin();
			ship != enemyFleet.end(); ship++) {
		if (ship->ReferAlive()) {//ターゲットの状態を確認
			UI.DrawShipOnTheMap(ship->ReferCoordX(),
				ship->ReferCoordZ(), true);//赤いマーク
		}
	}
}

void IngameDataManagement::DrawSea() {
	double mapX = PL.ReferMapX();
	double mapZ = PL.ReferMapZ();

	double MCPOX = MainCamera.ReferPrintOutX(mapX);
	double MCPOZ = MainCamera.ReferPrintOutZ(mapZ);

	int graphNumOnX = 2;
	int graphNumOnZ = 2;

	graphNumOnX = (int)(MainCamera.ReferCameraX() / mapX);
	graphNumOnZ = (int)(MainCamera.ReferCameraZ() / mapZ);

	//一枚の画像を使って連続描く
	{
		DrawRectGraph(//左上
			(int)(-MCPOX - mapX),
			(int)(-MCPOZ - mapZ),
			(int)0,
			(int)0,
			(int)mapX, (int)mapZ,
			*PL.ReferMapHandle(), FALSE, FALSE);

		DrawRectGraph(//上
			(int)(-MCPOX),
			(int)(-MCPOZ - mapZ),
			(int)0,
			(int)0,
			(int)mapX, (int)mapZ,
			*PL.ReferMapHandle(), FALSE, FALSE);

		DrawRectGraph(//右上
			(int)(-MCPOX + mapX),
			(int)(-MCPOZ - mapZ),
			(int)0,
			(int)0,
			(int)mapX, (int)mapZ,
			*PL.ReferMapHandle(), FALSE, FALSE);

		DrawRectGraph(//左
			(int)(-MCPOX - mapX),
			(int)(-MCPOZ),
			(int)0,
			(int)0,
			(int)mapX, (int)mapZ,
			*PL.ReferMapHandle(), FALSE, FALSE);

		DrawRectGraph(//中
			(int)(-MCPOX),
			(int)(-MCPOZ),
			(int)0,
			(int)0,
			(int)mapX, (int)mapZ,
			*PL.ReferMapHandle(), FALSE, FALSE);

		DrawRectGraph(//右
			(int)(-MCPOX + mapX),
			(int)(-MCPOZ),
			(int)0,
			(int)0,
			(int)mapX, (int)mapZ,
			*PL.ReferMapHandle(), FALSE, FALSE);

		DrawRectGraph(//左下
			(int)(-MCPOX - mapX),
			(int)(-MCPOZ + mapZ),
			(int)0,
			(int)0,
			(int)mapX, (int)mapZ,
			*PL.ReferMapHandle(), FALSE, FALSE);

		DrawRectGraph(//下
			(int)(-MCPOX),
			(int)(-MCPOZ + mapZ),
			(int)0,
			(int)0,
			(int)mapX, (int)mapZ,
			*PL.ReferMapHandle(), FALSE, FALSE);

		DrawRectGraph(//右下
			(int)(-MCPOX + mapX),
			(int)(-MCPOZ + mapZ),
			(int)0,
			(int)0,
			(int)mapX, (int)mapZ,
			*PL.ReferMapHandle(), FALSE, FALSE); 
	}
}

void IngameDataManagement::DrawThisList(std::list<Effect> *effectList) {
	if (!effectList->empty())//リスト状況を確認
		for (auto effect = effectList->begin();
			effect != effectList->end();
			effect++) {
		effect->Draw((int)MainCamera.ReferRealCameraX(),
			(int)MainCamera.ReferRealCameraZ());//相対座標を利用して描く
	}
}

/*船の下にあるエフェクトを描く*/
void IngameDataManagement::DrawEffectUnderShips() {
	DrawShipsShadow();//船の影を描く
	/*水泡演出*/
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 30);//透明度を下がる
	DrawThisList(&bubbleList);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 80);//透明度を下がる
	DrawThisList(&rippleList);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);//描画モードをもとに戻る
}

/*船の上にあるエフェクトを描く*/
void IngameDataManagement::DrawEffectBeyondShips() {
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);//透明度を下がる
	DrawThisList(&smokeList);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);//透明度を下がる
	DrawThisList(&explosionList);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);//描画モードをもとに戻る
}

void IngameDataManagement::DrawPointOfImpact() {
	auto ship = alliesFleet.begin();

	if (!ship->fireDataFigureUp.ReferLockOn())//ロックを使う時に落下地点を描画しません
		ship->ShowMePointOfImpact(MainCamera);//使わない時に描画する
}

void IngameDataManagement::DrawAmmo() {
	if (!shellList.empty())//リスト状況を確認
	for (auto shell = shellList.begin();
		shell != shellList.end();
		shell++) {
		shell->Draw((int)MainCamera.ReferRealCameraX(),
			(int)MainCamera.ReferRealCameraZ());//相対座標を利用して描画する
	}
}

//読み込み中画面
void IngameDataManagement::DrawLoading() {
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	unsigned int Cr;
	Cr = GetColor(255, 255, 255);
	DxLib::SetFontSize(30);

	DrawString((int)(0.85*Screen::SCREEN_X),
		(int)(0.85*Screen::SCREEN_Z), "LOADING...", Cr);

	DxLib::ScreenFlip();
}

void IngameDataManagement::DrawStatisticBoard() {
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	enum StaBoard {
		TOTAL_KILL = 0,
		TOTAL_MOVE,
		TOTAL_DAMAGE,
		TOTAL_DAMAGE_RECIEVED,
		MAX_DAMAGE,
		MAX_MOVE,
		MAX_HITRATE
	};

	double boardData[7] = { 0,0,0,0,0,0,0 };//初期化

	FILE *filePointer;//ファイルポインター
	char fileName[100] = "Data//TopScore//TopScore.txt";

	/*もし保存されたデータがある場合はそのまま読み込む*/
	if ((fopen_s(&filePointer, fileName, "r")) == 0) {

		for (int i = TOTAL_KILL; i <= MAX_HITRATE; i++)
			fscanf_s(filePointer, "%lf", &boardData[i]);
		fclose(filePointer);
	}

	fopen_s(&filePointer, fileName, "w");

	/*計算の部分*/
	if (shootCount == 0)/*ゼロの時も正しく表示できる*/
		shootCount = 1;
	hitRate = (double)hitCount / (double)shootCount;

	/*記録を更新*/
	boardData[TOTAL_KILL] += (double)killed;
	boardData[TOTAL_MOVE] += alliesFleet[0].ReferDistanceMoved();
	boardData[TOTAL_DAMAGE] += (double)damage;
	boardData[TOTAL_DAMAGE_RECIEVED] += (double)damageRecieved;
	if (damage > MAX_DAMAGE)
		boardData[MAX_DAMAGE] = damage;
	if (alliesFleet[0].ReferDistanceMoved() >
		boardData[MAX_MOVE])
		boardData[MAX_MOVE] = alliesFleet[0].ReferDistanceMoved();
	if (hitRate > boardData[MAX_HITRATE])
		boardData[MAX_HITRATE] = hitRate;

	for (int i = TOTAL_KILL; i <= MAX_HITRATE; i++)
		fprintf_s(filePointer, "%lf\n", boardData[i]);

	fclose(filePointer);//ファイルを閉じる

	/*描画色*/
	unsigned int Cr;
	Cr = GetColor(255, 255, 255);
	DxLib::SetFontSize(40);

	/*背景を描く*/
	DxLib::DrawExtendGraph(0, 0, Screen::SCREEN_X, Screen::SCREEN_Z, 
		*statisticBoard[StatisticBoard::SB_BACKGROUND], FALSE);

	if(win)
		DxLib::DrawExtendGraph(10, 20, 210, 91,
			*statisticBoard[StatisticBoard::WIN], TRUE);
	else
		DxLib::DrawExtendGraph(10, 20, 210, 91,
			*statisticBoard[StatisticBoard::LOSE], TRUE);

	/*今回のデータを描く*/
	DxLib::DrawFormatString(410, 115, Cr, "%2.3lf", hitRate);//命中率を表示
	DxLib::DrawFormatString(410, 225, Cr, "%d", damage);
	DxLib::DrawFormatString(410, 335, Cr, "%.1lf", alliesFleet[0].ReferDistanceMoved());
	DxLib::DrawFormatString(410, 445, Cr, "%d", killed);

	/*記録を描く*/
	DxLib::SetFontSize(26);
	for (int i = TOTAL_KILL; i <= MAX_HITRATE; i++)
		DxLib::DrawFormatString(940, 115 + i * 75, Cr, "%5.1lf",boardData[i]);

	DxLib::ScreenFlip();

	Sleep(1000);
	DxLib::WaitKey();
}

/****************************************************/
/*                     テスト　                     */
/****************************************************/
void IngameDataManagement::TEST() {
	alliesFleet.push_back(ShipMain());//新しい船を生成する
	auto ship = alliesFleet.begin();//イテレータを使って自分の船を選ぶ
	//ここの部分は初期化関数とファイル読み込みはまだ出来ていない
	//初期化関数は下のようにしたい
	

	ship->NewCoordX(640);//新しい座標をあげる
	ship->NewCoordZ(380);
	ship->NewCoordY(-10);
	ship->SetRadianOnZ(0);
	if (!ship->InifThisShip(&PL, ET, &SL, 4000, 1)) {
		DrawString(10, 10, "ファイル読み込む失敗", GetColor(255, 255, 255));
		DxLib::ScreenFlip();
		WaitKey();
		exit(1);
	}


	for (int i = 0; i < 2; i++) {
		enemyFleet.push_back(ShipMain());//テスト用敵船を生成する
		auto enemyShip = enemyFleet.end();//イテレータで船を選ぶ
		enemyShip--;//いつも最後の船を設置する
		double radian = (double)(rand() % 180) / 180.0*MathAndPhysics::PI;


		enemyShip->NewCoordX(2500 + (rand() % 400) * i);
		enemyShip->NewCoordZ(2000 + (rand() % 400) * i);
		enemyShip->NewCoordY(-10);
		enemyShip->SetRadianOnZ(radian);
		if (!enemyShip->InifThisShip(&PL, ET, &SL, 4000, i + 10)) {
			DrawString(10, 10, "ファイル読み込む失敗", GetColor(255, 255, 255));
			DxLib::ScreenFlip();
			WaitKey();
			exit(1);
		}
	}
}

void IngameDataManagement::TEST_DRAW() {
	unsigned int Cr;
	Cr = GetColor(255, 255, 255);
	auto ship = alliesFleet.begin();
	ship->TestDraw(MainCamera.ReferRealCameraX(),
		MainCamera.ReferRealCameraZ());
	char CharNum[255];

	DxLib::SetFontSize(15);
	ChangeFont("ＤＦＧ龍門石碑体");

	_gcvt_s(CharNum, ship->ReferSpeedOnZ() * 100, 10);
	DrawString(10, 10, "Speed", Cr);
	DrawString(60, 10, CharNum, Cr);
	if (ship->ReferReturnOn()) {
		DrawString(10, 30, "Alignment On", Cr);
	}
	else {
		DrawString(10, 30, "Alignment Off", Cr);
	}
	_gcvt_s(CharNum, ship->ReferChangingRadian(), 10);
	DrawString(10, 50, "Radian(C)", Cr);
	DrawString(100, 50, CharNum, Cr);
	_gcvt_s(CharNum, ship->ReferCoordX(), 10);
	DrawString(10, 70, "X", Cr);
	DrawString(30, 70, CharNum, Cr);
	_gcvt_s(CharNum, ship->ReferCoordZ(), 10);
	DrawString(10, 90, "Z", Cr);
	DrawString(30, 90, CharNum, Cr);
	_gcvt_s(CharNum, ship->ReferRadianOnZ(), 10);
	DrawString(10, 110, "Radian(R)", Cr);
	DrawString(100, 110, CharNum, Cr);
	_gcvt_s(CharNum, cos(ship->ReferRadianOnZ()), 10);
	DrawString(10, 130, "Cos", Cr);
	DrawString(50, 130, CharNum, Cr);
	_gcvt_s(CharNum, sin(ship->ReferRadianOnZ()), 10);
	DrawString(10, 150, "Sin", Cr);
	DrawString(50, 150, CharNum, Cr);
	_gcvt_s(CharNum, ship->ReferOutPutRate(), 10);
	DrawString(10, 170, "OutPutRate", Cr);
	DrawString(110, 170, CharNum, Cr);
}

void IngameDataManagement::TEST_WIN() {
	GameOver = true;
}

/****************************************************/
/*                   コントロール                   */
/****************************************************/
/*コマンドを受け取って、新たなものを生成する*/
void IngameDataManagement::Control() {
	int answer;

	answer = CUI.CheckChoice();

	/*重複クリックを防止*/
	if (answer == CommandSerial::INCREASE_OUTPUT ||
		answer == CommandSerial::DECREASE_OUTPUT ||
		answer == CommandSerial::MENU ||
		answer == CommandSerial::TURN_RETURN||
		answer == CommandSerial::CHANGE_CAMERA ||
		answer >= CommandSerial::SELECT)
		if (!CUI.ReferClickable())
			return;

	/*マウスでコマンドを受けなかった場合はキーボードからコマンドを受け取る*/
	if (answer == CommandSerial::NONE_COMMAND)
		answer = CT.GetCommand();

	/*コマンドがなければこの関数を終了する*/
	if (answer == CommandSerial::NONE_COMMAND)
		return;

	/*船を操作*/
	auto ship = alliesFleet.begin();
	ship->ControlThisShip(answer);

	CUI.SetClickTime();//クリックした時間を記録する
	
	/*select以前のコマンドの処理*/
	if (answer < CommandSerial::SELECT) {
		switch (answer) {
		case CommandSerial::SHOOT:TestShoot(&alliesFleet[0],true); break;/*射撃*/
		case CommandSerial::MENU:CUI.LetMeSeeMenu();break;/*メニュー状態変更*/
		case CommandSerial::TEST_VIEW_ON:TEST_SHOW_ON = !TEST_SHOW_ON; break;	/*テストビュー*/
		case CommandSerial::EXIT:GameOver = true; break;	/*ゲーム終了*/
		case CommandSerial::CHANGE_CAMERA:showLock = !showLock; break;
		}
	}

	else {
		ship->fireDataFigureUp.SetNumber(answer -
			CommandSerial::SELECT - CommandSerial::SELECT_RANGE);//ターゲットを設置する
		if (enemyFleet[answer -
			CommandSerial::SELECT - CommandSerial::SELECT_RANGE].ReferAlive()) {
			ship->fireDataFigureUp.LockOn_Switch();//ロック状態を変更
			ship->ResetReviseData();//修正データをリセット
			CUI.SetShootMenu(ship->fireDataFigureUp.ReferLockOn());//ＵＩを変更
		}
	}
}

/****************************************************/
/*                     移動関連                     */
/****************************************************/
void IngameDataManagement::MoveAll() {
	CheckShipsStatus();//船の状態を更新する
	MoveShips();//船を移動する
	MoveEffects();//エフェクトを移動する
	MoveAmmo();//弾を移動する

	SimpleHitDecision();//弾と船のあたり判定
	CrashDecision();//船の間のあたり判定
}

void IngameDataManagement::CheckShipListStatus(std::vector<ShipMain> *shipList) {
	if (!shipList->empty())
		for (auto ship = shipList->begin();
			ship != shipList->end();
			ship++) {
		ship->Update();//状態更新
	}
}
//船の状態を更新する
void IngameDataManagement::CheckShipsStatus() {
	CheckShipListStatus(&alliesFleet);
	CheckShipListStatus(&enemyFleet);
}

/*基底クラスを使って移動する*/
void IngameDataManagement::MoveShips() {
	if(!alliesFleet.empty())//リスト確認
	for (auto ship = alliesFleet.begin();
		ship != alliesFleet.end();
		ship++) {
		ship->Move();//友軍の船を移動する
	}
	if(!enemyFleet.empty())//リスト確認
	for (auto ship = enemyFleet.begin();
		ship != enemyFleet.end();
		ship++) {
		ship->Move();//敵の船を移動する
	}
}

void IngameDataManagement::MoveEffect(std::list<Effect> *effectList) {
	if (!effectList->empty())//リスト確認
		for (auto effect = effectList->begin();
			effect != effectList->end();
			effect++) {
		effect->Move();//水泡を船の後ろに移動する
	}
}

void IngameDataManagement::MoveEffects() {
	MoveEffect(&smokeList);
	MoveEffect(&bubbleList);
}

void IngameDataManagement::MoveAmmo() {
	if (!shellList.empty())//リスト確認
		for (auto shell = shellList.begin();
			shell != shellList.end();
			shell++) {
		shell->Move();
	}
}

/****************************************************/
/*                    データ関連                    */
/****************************************************/
void IngameDataManagement::Inif() {
	PL.InifForGame();//画像ローダー初期化
	UI.InifUI(&PL);//UI初期化
	ET.InifEffectTemplate(&PL);//エフェクトテンプレート初期化
	SL.Inif();//音声ローダー初期化
	CT.Inif(&SL);//キーボードコントローラー初期化
	CUI.IngameInif(&PL,&SL);//マウスコントローラー初期化

	/*統計ボードーの初期化*/
	for (int i = StatisticBoard::SB_BACKGROUND;
		i <= StatisticBoard::LOSE;
		i++)
		statisticBoard[i] =
		PL.RefetrStatisticBoardHandle(i);

	InifStatisticBoardData();

	TEST();
	alliesFleet[0].SetControled();//友軍艦隊の一番の操作権を取る
	CUI.InifShipList(&enemyFleet,false);
	CUI.InifShipList(&alliesFleet, true);
	CUI.SetNormalStatus();/*ここはテストバージョン*/
}

/*使ったメモリを解放する*/
void IngameDataManagement::Free() {
	PL.FREE_FOR_GAME();//動的メモリを解放、メモリ中のデータを削除
	SL.FreeAll();//動的メモリを解放、メモリ中のデータを削除
	CUI.Free();//ＵＩ部分の動的メモリを解放する
	DestroyShips();//船が利用した動的メモリを解放する

	using namespace std;

	/*標準ライブラリメモリ解放*/
	/*vector解放*/
	alliesFleet.clear();
	vector<ShipMain>().swap(alliesFleet);
	enemyFleet.clear();
	vector<ShipMain>().swap(enemyFleet);

	/*リスト解放*/
	shellList.clear();
	bombList.clear();
	torpedoList.clear();
	bubbleList.clear();
	smokeList.clear();
	explosionList.clear();
	rippleList.clear();
}

/*制限時間超えたものを消す*/
void IngameDataManagement::DeleteUseless() {
	DeleteUselessEffect();//時間切りエフェクトを消す
	DeleteUselessAmmo();//海に落ちた砲弾を消す
	RemoveDestroyedShip();//沈んだ船を沈むリストに追加する
}

void IngameDataManagement::DestroyShips() {
	DestroyThisTeam(&alliesFleet);//友軍艦隊のメモリ解放
	DestroyThisTeam(&enemyFleet);//敵艦隊のメモリ解放
}

void IngameDataManagement::DestroyThisTeam(std::vector<ShipMain> *shipList) {
	if (!shipList->empty()) {
		for (auto ship = shipList->begin();
			ship != shipList->end();
			ship++) {
			ship->DestroyMemory();//メモリ解放
		}
	}
}

void IngameDataManagement::CheckTeamStatus() {
	/*友軍の状態を確認する*/
	CheckAlliesStatus();
	/*敵の状態を確認する*/
	CheckEnemyStatus();
}

void IngameDataManagement::CheckAlliesStatus() {
	for (auto ship = alliesFleet.begin();
		ship != alliesFleet.end();
		ship++) {
		if (ship->ReferAlive())
			return;
	}
	win = false;//友軍艦隊は全滅されたら負けです
	TEST_WIN();/*終わる状態を設置*/
}

void IngameDataManagement::CheckEnemyStatus() {
	for (auto ship = enemyFleet.begin();
		ship != enemyFleet.end();
		ship++) {
		if (ship->ReferAlive())
			return;
	}
	win = true;//敵軍艦隊は全滅されたら負けです
	TEST_WIN();/*終わる状態を設置*/
}

void IngameDataManagement::CheckAndPlaySound() {
	auto ship = alliesFleet.begin();
	ship->CheckAndPlaySound();//自分の船に関連する音を流す
}

/****************************************************/
/*                  エフェクト関連                  */
/****************************************************/
void IngameDataManagement::GetNewEffect() {
	/*進行中の水泡生成*/
	NewEffectForShips(alliesFleet);
	NewEffectForShips(enemyFleet);
}

void IngameDataManagement::NewEffectForShips(std::vector<ShipMain> shipList) {
	/*進行中の水泡生成*/
	if (!shipList.empty()) {//リスト確認
		for (auto ship = shipList.begin();
			ship != shipList.end();
			ship++) {
			if(ship->ReferAlive())
			if (abs(ship->ReferSpeedOnZ()) > 0.05 && rand() % 3 == 0
				&& rand() % 100 > ship->ReferSpeedOnZ() * 10) {//一定の速度があれば、確率で生成する
				//for(int i = 0; i < ship->ReferBubbleCount(); i++)
				for (int i = 0; i < ship->ReferBubblePointCount(); i++)
					//すべてのエフェクトポイントからエフェクトを生成する
					bubbleList.push_back(ship->NewBubble(i));//リストの末に追加する
			}
		}
		for (auto ship = shipList.begin();
			ship != shipList.end();
			ship++) {
			if (ship->ReferAlive())
			if (ship->ReferOutPutRate() != 0) {
				if ((rand() % 8 < abs(ship->ReferSpeedOnZ()) * 10)
					&& rand() % 4 == 0) {//確率で生成する
					//for(int i = 0; i < ship->ReferSmokeCount(); i++)
					for (int i = 0; i < 2; i++)
						smokeList.push_back(ship->NewSmoke(i));//リストの最後に追加する
				}
			}
		}
	}
}

/*新しい爆発エフェクトを生成する*/
void IngameDataManagement::NewExplosion(Coordinate2D<double> Coord) {
	/*画像の向きがランダムです*/
	double radian = (double)(rand() % 180) * 1.0 / 180.0 * MathAndPhysics::PI;
	Effect effect(false, 500, radian, 0, 0, 0, Coord.x, Coord.z,
		PL.ReferEffectList(TypeOfEffect::EXPLOSION), true,
		0.04 * (double)(rand()%4), 1.05);

	explosionList.push_back(effect);//生成されたものをリストの後ろに追加する
}

void IngameDataManagement::NewRipple(double coordX, double coordZ) {
	Effect ripple(false, 1000, 0, 0, 0, 0, coordX, coordZ,
		PL.ReferBubbleHandle(), true, 0.3, 1.09);
	//砲弾が消失したところに一秒後消すエフェクトを生成する
	rippleList.push_back(ripple);//生成したエフェクトをリストの末に追加する
}


void IngameDataManagement::DeleteUselessEffect() {
	CheckAndCleanThisEffectList(&bubbleList);//水泡リストの中のいらないものを消す
	CheckAndCleanThisEffectList(&smokeList);//雲リストの中のいらないものを消す
	CheckAndCleanThisEffectList(&rippleList);//
	CheckAndCleanThisEffectList(&explosionList);//爆発リストをクリア
}

void IngameDataManagement::CheckAndCleanThisEffectList(std::list<Effect> *effectList) {
	if (!effectList->empty())//空きの場合はスキップする
		for (auto effect = effectList->begin();
			effect != effectList->end();
			effect++) {
		effect->Update();//状態更新
		if (effect->ReferTimeUp()) {//時間超えたら
			effect = effectList->erase(effect);//リストから削除
		}
		if (effectList->empty() || effect == effectList->end())
			//もしリストの末になったら繰り返すを終了する
			break;
	}
}

/****************************************************/
/*                     射撃関連                     */
/****************************************************/

void IngameDataManagement::TestShoot(ShipMain *ship,bool me) {
	FiringData FD;
	//ゲーム中では、敵が射撃する時に全部trueにする
	FD.isThisMainWeapon = true;
	for (int i = 0; i < 8; i++) {
		FD.selected[i] = true;//武器を選ぶ
	}
	InputNewAmmo(ship, FD, me);//リストに新しい弾を追加する
}

void IngameDataManagement::LockAndRefresh() {
	counter++;
	if (counter >= 60)//60フレームごとにリセットする
		counter = counter % 60;

	shouldIRender = counter % 20 == 0 ? true : false;//毎秒三回レンダーする

	CheckThisTeamLock(&alliesFleet, enemyFleet);
	CheckThisTeamLock(&enemyFleet, alliesFleet);

}

void IngameDataManagement::CheckThisTeamLock(std::vector<ShipMain> *shipList,
	std::vector<ShipMain> enemyList) {
	if (!shipList->empty() && !enemyList.empty()) {
		for (auto ship = shipList->begin();
			ship != shipList->end();
			ship++) {
			if(ship->ReferAlive())
			if (ship->fireDataFigureUp.ReferLockOn()) {//もしロックは使っていれば
				ship->TestLock(&enemyList[ship->fireDataFigureUp.ReferTarget()],
					shouldIRender);//選んだ敵をロックする
				//船２を目標にしてロックする
			}
		}
	}
}

void IngameDataManagement::InputNewAmmo(ShipMain *SM, FiringData FD ,bool me) {
	int weaponAmount = SM->ReferWeaponCount(FD.isThisMainWeapon);//選んだ武器を確認する
	for (int i = 0; i < weaponAmount; i++) {
		if (FD.selected[i]) {
			if (SM->IsThisOneUsable(i, FD.isThisMainWeapon)) {//射撃が可能であれば
				shellList.push_back(SM->Shoot(i,FD.isThisMainWeapon));//新しい弾を追加する
				if (me)
					shootCount++;
			}
		}
	}
}

void IngameDataManagement::DeleteUselessAmmo() {
	if (!shellList.empty())//リスト確認
		for (auto shell = shellList.begin();
			shell != shellList.end();
			shell++) {
		if (shell->FallIntoWater()|| !shell->ReferUsable()) {
			if (shell->FallIntoWater())//もし弾が水中に落ちったら
				NewRipple(shell->ReferCoordX(), shell->ReferCoordZ());//弾を消す前に水泡を生成する
			shell = shellList.erase(shell);//弾を消す
		}
		if (shellList.empty() || shell == shellList.end())
			break;
	}
}


/****************************************************/
/*                     あたり判定                   */
/****************************************************/
void IngameDataManagement::CrashDecision() {
	if (!alliesFleet.empty() && !enemyFleet.empty()) {//リスト確認
		for (auto ship1 = alliesFleet.begin();
			ship1 != alliesFleet.end();
			ship1++) {
			if (ship1->ReferAlive())//生きる状態確認
			for (auto ship2 = enemyFleet.begin();
				ship2 != enemyFleet.end();
				ship2++) {
				if (ship2->ReferAlive())//生きる状態確認
				if (PointsToCollisionbox(&*ship1, &*ship2)) {
					ship1->Unmove(); ship1->ResetStatus();//船を停止する
					ship2->Unmove(); ship2->ResetStatus();
				}
			}
		}
		//敵同士のチェック
		for (auto ship1 = enemyFleet.begin();
			ship1 != enemyFleet.end();
			ship1++) {
			if (ship1->ReferAlive())//生きる状態確認
				for (auto ship2 = enemyFleet.begin();
					ship2 != enemyFleet.end();
					ship2++) {
				if (ship1 == ship2)//自分とのあたり判定をしません
					continue;
				if (ship2->ReferAlive())//生きる状態確認
					if (PointsToCollisionbox(&*ship1, &*ship2)) {
						ship1->Unmove(); ship1->ResetStatus();//船を停止する
						ship2->Unmove(); ship2->ResetStatus();
					}
			}
		}
		//友軍同士のチェック
		for (auto ship1 = alliesFleet.begin();
			ship1 != alliesFleet.end();
			ship1++) {
			if (ship1->ReferAlive())//生きる状態確認
				for (auto ship2 = alliesFleet.begin();
					ship2 != alliesFleet.end();
					ship2++) {
				if (ship1 == ship2)//自分とのあたり判定をしません
					continue;
				if (ship2->ReferAlive())//生きる状態確認
					if (PointsToCollisionbox(&*ship1, &*ship2)) {
						ship1->Unmove(); ship1->ResetStatus();//船を停止する
						ship2->Unmove(); ship2->ResetStatus();
					}
			}
		}
	}
}

bool IngameDataManagement::PointsToCollisionbox(ShipMain *ship1, ShipMain *ship2) {
	for (int i = 0; i < ship1->ReferCollisionPointAmount(); i++) {
		if (!ship2->ReferAlive())//船にの状態を確認
			break;//死んだら終わる

		Coordinate<double> temp;
		Coordinate2D<double> temp2D;
		temp = ship1->ReferCoord();//船1にの状態を取る
		temp2D = ship1->ReferCollisionPoint(i);//船1のあたり判定点を取る

		/*本物の座標を計算する*/
		temp.x = temp.x - cos(ship1->ReferRadianOnZ())
			* temp2D.x + sin(ship1->ReferRadianOnZ()) * temp2D.z;
		temp.z = temp.z - cos(ship1->ReferRadianOnZ())
			* temp2D.z - sin(ship1->ReferRadianOnZ()) * temp2D.x;
		temp.y = 5;
		
		//点と長方形のあたり判定関数を利用して計算する
		if (crash3DtoPoint(ship2->ReferCoord(),
			temp, ship2->ReferShipCrashSize(),
			ship2->ReferRadianOnZ())) {
			return true;
		}
	}
	return false;
}

void IngameDataManagement::SimpleHitDecision() {
	if (!shellList.empty())//リスト空き確認
		for (auto shell = shellList.begin();
			shell != shellList.end();
			shell++) {
		CheckThisTeamDecision(&alliesFleet, &*shell);//友軍のチェック
		CheckThisTeamDecision(&enemyFleet, &*shell);//敵のチェック
	}
}



void IngameDataManagement::CheckThisTeamDecision(std::vector<ShipMain> *shipList,
	Ammo *shell) {
	if (!shipList->empty())//リスト空き確認
		for (auto ship = shipList->begin();
			ship != shipList->end();
			ship++) {
		if (shell->ReferSerialNumber() != ship->ReferSerialNumber()) {
			//弾と船のあたり判定を行う
			if (crash3DtoPoint(ship->ReferCoord(),
				shell->ReferCoord(),ship->ReferShipCrashSize(),
				ship->ReferRadianOnZ()))
			if(ship->ReferAlive()){
				//当たったら
				ship->SufferDamage((int)shell->ReferDamage());//ダメージを与える
				Coordinate2D<double> C2D = { shell->ReferCoordX(),
				shell->ReferCoordZ() };
				NewExplosion(C2D);//当たったところに爆発エフェクトを生成
				shell->Unusable();//弾が使えなくなる
				
				if (shell->ReferSerialNumber() == 1) {
					hitCount++;//ヒット数増加
					damage += (int)shell->ReferDamage();//ダメージ数増加
					if (!ship->ReferAlive()) {
						if (alliesFleet[0].fireDataFigureUp.ReferLockOn() == true) {
							alliesFleet[0].fireDataFigureUp.LockOn_Switch();//ロック状態を変更
							ship->ResetReviseData();//修正データをリセット
							CUI.SetShootMenu(ship->fireDataFigureUp.ReferLockOn());//ＵＩを変更
						}
						killed++;
					}
				}
				return;
			}
		}
	}
}

void IngameDataManagement::RemoveDestroyedShip() {
	CheckTeamA(&alliesFleet);
	CheckTeamA(&enemyFleet);
}

/*この関数は船が撃沈した後に沈む演出を正常に行うために作りました*/
void IngameDataManagement::CheckTeamA(std::vector<ShipMain> *shipList) {
	if(!shipList->empty()){//リスト空き確認
		for (auto ship = shipList->begin();
			ship != shipList->end();
			ship++) {
			if (!ship->ReferAlive()) {
				sinkingShip.push_back(*ship);//沈んでいるリストに追加する
			}
		}
	}
}