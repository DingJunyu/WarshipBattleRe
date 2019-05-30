#include "IngameDataManagement.h"
/*イテレータを使う前に必ず容器の状況を確認します！！！！！*/
using namespace DxLib;
IngameDataManagement::~IngameDataManagement()
{
}

/*メインルート*/
void IngameDataManagement::Update() {
	AIUpdate();//AIでデータを更新すｒう
	LockAndRefresh();//ロック状態を更新すｒう

	/*カメラ状態を設置する*/
	if (alliesFleet[0].fireDataFigureUp.ReferLockOn() && showLock == true)
		MainCamera.GetXZ(ReferTargetX(alliesFleet[0].fireDataFigureUp.ReferTarget()),
			ReferTargetZ(alliesFleet[0].fireDataFigureUp.ReferTarget()));
	else
		MainCamera.GetXZ(ReferPlayerX(), ReferPlayerZ());//カメラ座標を更新
		
	Control();//コマンドを受け取って、船の状態を変更する
	GetNewEffect();//エフェクトを生成する
	MoveAll();//移動、状態更新

	SinkingListUpdate();

	if (!GameEnd)
		CheckTeamStatus();//各艦隊の状態を確認し、一方が殲滅されたらゲーム終了

	DeleteUseless();//入らないものを消す
	RemoveSinkedShip();

	if (GameEnd)
		EndTheGame();

	CheckAndPlaySound();
	if (ShouldIDraw())
		DrawAll();//全部更新した後画面を描く
	shouldIDraw = FC.Wait();
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
	LetEveryOneMove();//すべての船の状態を更新する
	LetEveryOneLockOn();//ターゲットを選択する
	LetEveryOneShoot();//射撃する
}

void IngameDataManagement::LetFlagShipMove() {
	if (!enemyFlagShip->ReferAlive())//もしフラグシープはまだ生きてあれば
		return;
	flagShipAI.LetUsGo(enemyFlagShip, &alliesFleet[0]);//プレーヤーが操作している船をターゲットに設定し移動する
	enemyFlagShip->SetChangingDirect(flagShipAI.ReferRadianNeededNow());//角度を更新する
	enemyFlagShip->SetEngineOutPutRate(flagShipAI.ReferSpeedInNeed());//出力を更新する
}

void IngameDataManagement::LetEveryOneMove() {
	ControlThisListMove(&alliesFleet, &AI);//友軍艦隊を移動する
	ControlThisListMove(&enemyFleet, &AI);//敵軍艦隊を移動する
}

void IngameDataManagement::ControlThisListMove(std::vector<ShipMain> *shipList,
	ArtificialIntelligence *AI) {	
	bool first = true;
	if (!shipList->empty() && shipList->begin()!=shipList->end()) {
		for (auto ship = shipList->begin();
			ship != shipList->end();
			ship++) {
			if (first) {//一番目をスキップする
				first = false;
				continue;
			}
			if (!ship->ReferAlive())//沈んだらスキップする
				continue;
			if (&*ship == enemyFlagShip)//フラグシープだったらスキップする
				continue;
			ship--;
			auto prevShip = ship;//前の船を設定する
			ship++;
			while (1) {
				if (prevShip->ReferAlive())//前の船はまだ生きていればそのまま使う
					break;
				if (prevShip == shipList->begin()) {//もし一番前の船も沈んだら
					enemyFlagShip = &*ship;//今の船をフラッグシープに設定する
					return;//更新をやめる
				}
				if (prevShip != shipList->begin())//まだ前に移動できれば
					prevShip--;
			}
			AI->Move(*ship, *prevShip);//移動する
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
			ship->fireDataFigureUp.LockOn_Switch();//ロック状態を更新する
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
			(!ship->ReferControled()||
			autoFire) &&//プレーヤーの船じゃないもしくはプレーヤーは自動射撃使っていない
			ship->ReferCanIShoot()) {//射撃範囲内にいる
			if (ship->ReferSerialNumber() != 0)
				TestShoot(&*ship, false);//射撃
			else
				TestShoot(&*ship, true);
		}
	}
}

/****************************************************/
/*                     描画関連                     */
/****************************************************/
void IngameDataManagement::DrawAll() {
	SetDrawScreen(DX_SCREEN_BACK);//裏画面に描画する
	ClearDrawScreen();//画面をクリアする

	auto ship = alliesFleet.begin();//イテレータで操作している船のステータスを取る

	/*海を描く*/
	DrawSea_New_Extend();
	DrawMesh_Sea_Extend();

	//船の下のエフェクトを描画する
	DrawEffectUnderShips();
	/*船のしたにあるUIを描画する*/
	UI.DrawUIUnderShip(ship->fireDataFigureUp.ReferLockOn(),
		enemyFleet[ship->fireDataFigureUp.ReferTarget()].ReferCoord2D_d(),
		alliesFleet[0].ReferCoord2D_d(),
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
	UI.DrawUINeedInput(&*ship, autoFire,//船、自動射撃状態
		(alliesFleet[0].fireDataFigureUp.ReferLockOn() && showLock));//カメラ目標
	//船のステータスに関係があるUIを描画する
	DrawShipsOnMiniMap();//ミニマップを描く
	/*クリック可能なUI*/
	CUI.Draw();//ボタンを描く
	CUI.DrawNeedInput(ship->fireDataFigureUp.ReferLockOn(), 
		ship->fireDataFigureUp.ReferTarget());//引数が必要な部分を描く
	DrawHPBar();
	

	/*テストインフォメーション*/
	if (TEST_SHOW_ON)
		TEST_DRAW();

	if (GameEnd)
		UI.DrawWinOrLose(win);

	DrawErrorList();
	
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

	if (!sinkingShip.empty()) {
		for (auto ship = sinkingShip.begin();
			ship != sinkingShip.end(); ship++) {
			ship->DrawSub(MainCamera);
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
	if (!sinkingShip.empty()) {
		for (auto ship = sinkingShip.begin();
			ship != sinkingShip.end(); ship++) {
			ship->DrawSubShadow(MainCamera);
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

void IngameDataManagement::SinkingListUpdate() {
	if (!sinkingShip.empty()) {
		for (auto ship = sinkingShip.begin();
			ship != sinkingShip.end(); ship++) {
			SinkingShipUpdate(&*ship);
		}
	}
}

void IngameDataManagement::SinkingShipUpdate(ShipMain *ship) {
	int num = rand() % 2;

	ship->countSinkingFlame();

	if (num) {
		Coordinate2D<double> temp = ship->ReferCoord2D_d();

		temp.x -= ship->ReferShipCrashSize().x / 16;
		temp.z -= ship->ReferShipCrashSize().z / 16;

		double randX, randZ;

		randX = (double)(rand() % (int)ship->ReferShipCrashSize().x/2);
		if (rand() % 2 == 0)
			randX = -randX;
		randZ = (double)(rand() % (int)ship->ReferShipCrashSize().z/2);
		if (rand() % 2 == 0)
			randZ = -randZ;

		temp.x += cos(ship->ReferRadianOnZ()) * randX -
			sin(ship->ReferRadianOnZ()) * randZ;
		temp.z += cos(ship->ReferRadianOnZ()) * randZ +
			sin(ship->ReferRadianOnZ()) * randX;

		NewExplosion(temp);
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

void IngameDataManagement::DrawSea_New() {
	double mapX = PL.ReferMapX();
	double mapZ = PL.ReferMapZ();
	double MCPOX, MCPOZ;

	if (MainCamera.ReferCameraX() > 0)
		MCPOX = Screen::SCREEN_X / 2 - abs(MainCamera.ReferPrintOutX(mapX / 2));
	else
		MCPOX = Screen::SCREEN_X / 2 + abs(MainCamera.ReferPrintOutX(mapX / 2));

	if (MainCamera.ReferCameraZ() > 0)
		MCPOZ = Screen::SCREEN_Z / 2 - abs(MainCamera.ReferPrintOutZ(mapZ / 2));
	else
		MCPOZ = Screen::SCREEN_Z / 2 + abs(MainCamera.ReferPrintOutZ(mapZ / 2));

	DrawGraph((int)MCPOX, (int)MCPOZ, *PL.ReferMapHandle(), TRUE);
	DrawGraph((int)MCPOX - (int)mapX, (int)MCPOZ, *PL.ReferMapHandle(), TRUE);
	DrawGraph((int)MCPOX, (int)MCPOZ - (int)mapZ, *PL.ReferMapHandle(), TRUE);
	DrawGraph((int)MCPOX - (int)mapX, (int)MCPOZ - (int)mapZ, *PL.ReferMapHandle(), TRUE);
}

void IngameDataManagement::DrawSea_New_Extend() {
	double mapX = PL.ReferMapX();
	double mapZ = PL.ReferMapZ();
	double MCPOX, MCPOZ;
	double multiple = MainCamera.ReferZoomRatio();

	if (MainCamera.ReferCameraX() > 0)
		MCPOX = Screen::SCREEN_X / 2 -
		abs(MainCamera.ReferPrintOutX(mapX * multiple / 2));
	else
		MCPOX =
		abs(MainCamera.ReferPrintOutX(mapX * multiple / 2));

	if (MainCamera.ReferCameraZ() > 0)
		MCPOZ = Screen::SCREEN_Z / 2 -
		abs(MainCamera.ReferPrintOutZ(mapZ * multiple / 2));
	else
		MCPOZ =
		abs(MainCamera.ReferPrintOutZ(mapZ * multiple / 2));

	DrawExtendGraph((int)MCPOX, (int)MCPOZ,
		(int)(MCPOX + mapX * multiple), 
		(int)(MCPOZ + mapZ * multiple),
		*PL.ReferMapHandle(), TRUE);
	DrawExtendGraph((int)(MCPOX - mapX * multiple),
		(int)MCPOZ,
		(int)(MCPOX),
		(int)(MCPOZ + mapZ  * multiple),
		*PL.ReferMapHandle(), TRUE);
	DrawExtendGraph((int)MCPOX, 
		(int)(MCPOZ - mapZ * multiple),
		(int)(MCPOX + mapX * multiple),
		(int)(MCPOZ),
		*PL.ReferMapHandle(), TRUE);
	DrawExtendGraph((int)(MCPOX - mapX * multiple),
		(int)(MCPOZ - mapZ * multiple),
		(int)(MCPOX),
		(int)(MCPOZ),
		*PL.ReferMapHandle(), TRUE);
}

void IngameDataManagement::DrawMesh_Sea_Extend() {
	double mapX = PL.ReferMapX();
	double mapZ = PL.ReferMapZ();
	double multiple = MainCamera.ReferZoomRatio();

	double MCPOX;
	double MCPOZ;

	if (MainCamera.ReferCameraX() > 0)
		MCPOX = Screen::SCREEN_X / 2 -
		abs(MainCamera.ReferPrintOutX(mapX * multiple / 2)) + flameCount;
	else
		MCPOX =
		abs(MainCamera.ReferPrintOutX(mapX * multiple / 2)) + flameCount;

	if (MainCamera.ReferCameraZ() > 0)
		MCPOZ = Screen::SCREEN_Z / 2 -
		abs(MainCamera.ReferPrintOutZ(mapZ * multiple / 2)) + flameCount;
	else
		MCPOZ =
		abs(MainCamera.ReferPrintOutZ(mapZ * multiple / 2)) + flameCount;


	if (flameCount >= mapX / 4)
		flameCount = fmod(flameCount, mapX / 4);

	flameCount += .05 * MainCamera.ReferZoomRatio();

	SetTrans(90);//透明度を下がる
	DrawExtendGraph((int)MCPOX, (int)MCPOZ,
		(int)(MCPOX + mapX * multiple),
		(int)(MCPOZ + mapZ * multiple),
		*PL.ReferNoiseHandle(), TRUE);
	DrawExtendGraph((int)(MCPOX - mapX * multiple),
		(int)MCPOZ,
		(int)(MCPOX),
		(int)(MCPOZ + mapX * multiple),
		*PL.ReferNoiseHandle(), TRUE);
	DrawExtendGraph((int)MCPOX,
		(int)(MCPOZ - mapZ * multiple),
		(int)(MCPOX + mapX * multiple), 
		(int)(MCPOZ),
		*PL.ReferNoiseHandle(), TRUE);
	DrawExtendGraph((int)(MCPOX - mapX * multiple),
		(int)(MCPOZ - mapZ * multiple),
		(int)(MCPOX),
		(int)(MCPOZ),
		*PL.ReferNoiseHandle(), TRUE);
	ResetTrans();//描画モードをもとに戻る
}

void IngameDataManagement::DrawMesh_Sea() {
	double mapX = PL.ReferMapX();
	double mapZ = PL.ReferMapZ();

	double MCPOX;
	double MCPOZ;

	MCPOX = Screen::SCREEN_X / 2 - MainCamera.ReferPrintOutX(mapX / 2);

	MCPOZ = Screen::SCREEN_Z / 2 - MainCamera.ReferPrintOutZ(mapZ / 2);
	
	double realX = fmod(MCPOX + flameCount, mapX / 2);
	double realZ = fmod(MCPOZ + flameCount, mapZ / 2);

	if (flameCount >= PL.ReferMapX())
		flameCount = fmod(flameCount, (double)PL.ReferMapX());

	flameCount += .05;

	SetTrans(90);//透明度を下がる
	DrawGraph((int)realX,
		(int)realZ,
		*PL.ReferNoiseHandle(), TRUE);
	DrawGraph((int)realX - PL.ReferMapX(),
		(int)realZ,
		*PL.ReferNoiseHandle(), TRUE);
	DrawGraph((int)realX,
		(int)realZ - PL.ReferMapZ(),
		*PL.ReferNoiseHandle(), TRUE);
	DrawGraph((int)realX - PL.ReferMapX(),
		(int)realZ - PL.ReferMapZ(),
		*PL.ReferNoiseHandle(), TRUE);
	ResetTrans();//描画モードをもとに戻る
}


void IngameDataManagement::DrawThisList(std::list<Effect> *effectList) {
	if (!effectList->empty())//リスト状況を確認
		for (auto effect = effectList->begin();
			effect != effectList->end();
			effect++) {
		effect->Draw(MainCamera);//相対座標を利用して描く
	}
}

/*船の下にあるエフェクトを描く*/
void IngameDataManagement::DrawEffectUnderShips() {
	DrawShipsShadow();//船の影を描く
	/*水泡演出*/
	SetTrans(90);//透明度を下がる
	DrawThisList(&bubbleList);
	SetTrans(80);//透明度を下がる
	DrawThisList(&rippleList);
	ResetTrans();//描画モードをもとに戻る
}

/*船の上にあるエフェクトを描く*/
void IngameDataManagement::DrawEffectBeyondShips() {
	SetTrans(140);//透明度を下がる
	DrawThisList(&smokeList);
	SetTrans(210);//透明度を下がる
	DrawThisList(&explosionList);
	ResetTrans();//描画モードをもとに戻る
}

void IngameDataManagement::DrawPointOfImpact() {
	auto ship = alliesFleet.begin();

//	if (!ship->fireDataFigureUp.ReferLockOn())//ロックを使う時に落下地点を描画しません
		ship->ShowMePointOfImpact(MainCamera);//使わない時に描画する
}

void IngameDataManagement::DrawHPBar() {
	DrawBarForATeam(alliesFleet,true);
	DrawBarForATeam(enemyFleet,false);
}

void IngameDataManagement::DrawBarForATeam(std::vector<ShipMain> shipList, bool left) {
	if (!shipList.empty()) {
		int count = 0;

		for (auto ship = shipList.begin();
			ship != shipList.end();
			ship++) {

			Coordinate2D<double> coord;
			coord.x = left ? BUTTON_POSITION_INGAME::SHIP_MARK_LEFT_X_COORD
				: BUTTON_POSITION_INGAME::SHIP_MARK_RIGHT_X_COORD;

			coord.z = BUTTON_POSITION_INGAME::SHIP_MARK_NEXT_Z_COORD*(count + 1);

			UI.DrawHPBar(coord, ship->ReferHP(),ship->ReferMaxHP());
			
			count++;
		}
	}
}


void IngameDataManagement::DrawAmmo() {
	if (!shellList.empty())//リスト状況を確認
	for (auto shell = shellList.begin();
		shell != shellList.end();
		shell++) {
		shell->Draw(MainCamera);//相対座標を利用して描画する
	}
}

//読み込み中画面
void IngameDataManagement::DrawLoading() {
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	unsigned int Cr;
	Cr = GetColor(255, 255, 255);
	DxLib::SetFontSize(30);

	DrawString((int)(0.8*Screen::SCREEN_X),
		(int)(0.8*Screen::SCREEN_Z), "LOADING...", Cr);

	DxLib::ScreenFlip();
}

void IngameDataManagement::DrawLoading(int num) {
	ClearDrawScreen();

	unsigned int Cr;
	Cr = GetColor(255, 255, 255);
	DxLib::SetFontSize(30);

	DrawString((int)(0.8*Screen::SCREEN_X),
		(int)(0.8*Screen::SCREEN_Z), "LOADING...", Cr);
	DrawFormatString((int)(0.8*Screen::SCREEN_X),
		(int)(0.85*Screen::SCREEN_Z), Cr, "%d/%d", num, asyncLoadNum);
	DrawBox(0, (int)(0.9*Screen::SCREEN_Z),
		(int)((double)num / (double)asyncLoadNum * Screen::SCREEN_X), 
		(int)(0.95*Screen::SCREEN_Z),
		Cr, TRUE);

	DxLib::ScreenFlip();
}

bool IngameDataManagement::FormationBoard() {
	while (1) {
		if (ProcessMessage() == -1)//ウィンドを閉じる時正しく終了できるようにする
			break;

		int ans = CommandSerial::NONE_COMMAND;//オーダーを取る

		if (CUI.ReferClickable())
			ans = CUI.CheckChoice();

		CheckErrorList();

		if (ans == GAME_START) {//もしゲームスタートを押したら
			int flagShipCount = teamA[flagShipNum].ReferNumber();
			if (flagShipCount != 0 &&//同時に両チームは空でなければ
				teamBCount != 0) {
				break;
			}
			if (teamACount == 0 || teamBCount == 0)
			{
				msgList.push_back(Fleet_Empty());
			}
			if (flagShipCount == 0 && teamACount != 0 && teamBCount != 0) {
				msgList.push_back(FlagShip_Empty());
			}
		}

		if (ans == CommandSerial::EXIT_IN_FORMATION) {
			shouldIContinue = false;
			return false;
		}

		if (ans >= CommandSerial::SELECT_IN_FORMATION &&
			ans < CommandSerial::SELECT_IN_FORMATION + 16) {
			CUI.SetClickTime();//押す時間を記録する
			ans -= 60;//番号を記録する
			if (ans % 2 == 0) {
				if (ans >= 8 && teamBCount < maxCountInATeam) {
					teamBCount++;
					teamB[(ans - 8) / 2].Plus();
				}
				if (ans < 8 && teamACount < maxCountInATeam) {
					teamACount++;
					teamA[ans / 2].Plus();
				}
			}
			if (ans % 2 == 1) {
				if (ans >= 8 && teamBCount > 0) {
					if (teamB[(ans - 8) / 2].ReferNumber() > 0) {
						teamBCount--;
						teamB[(ans - 8) / 2].Minus();
					}
				}
				if (ans < 8 && teamACount > 0) {
					if (teamA[ans / 2].ReferNumber() > 0) {
						teamACount--;
						teamA[ans / 2].Minus();
					}
				}
			}
		}

		if (ans == CommandSerial::RANDOM_FORMATION) {
			CUI.SetClickTime();
			ClearTeam();//ランダム前にクリアする
			int temp = rand() % 9 + 1;
			SetRandom(temp, 0, true, &teamACount);
			SetRandom(temp, 0, false, &teamBCount);
		}

		if (ans == CommandSerial::CLEAR_FORMATION) {
			CUI.SetClickTime();
			ClearTeam();
		}

		/*フラグシープ選択部分*/
		Coordinate2D<int> mousePos;
		if(GetMouseInput() == MOUSE_INPUT_LEFT)
			GetMousePoint(&mousePos.x, &mousePos.z);
		for (int i = 0; i < 4; i++) {
			if (teamA[i].Clicked({ 70,100 + 120 * i }, mousePos)) {
				int tempF = i;
				for (int j = 0; j < 4; j++) {
					if (tempF == j)continue;
					if (teamA[j].ReferFlag())
						teamA[j].ChangeFlagStatus();
				}
				flagShipNum = i;
			}
		}

		DrawFormationBoard();
	}
	return true;
}

bool IngameDataManagement::RegisterTeam() {
	int count = 0;
	Coordinate<double> coord{ -500, -10, 200 };

	for (int i = 0; i < teamA[flagShipNum].ReferNumber(); i++) {
		alliesFleet.push_back(ShipMain());
		auto ship = alliesFleet.end();
		ship--;

		coord.x -= 300;//戦艦の間の間隔を取る

		if (!GetShipDataFromFile(coord, 0,
			&*ship, teamA[flagShipNum].ship.ReferShipType(),
			count)) {
			return false;
		}
		count++;
	}

	for (int i = 0; i < 4; i++) {
		if (i == flagShipNum)
			continue;
		for (int j = 0; j < teamA[i].ReferNumber(); j++) {

			alliesFleet.push_back(ShipMain());
			auto ship = alliesFleet.end();
			ship--;
			coord.x -= 300;//戦艦の間の間隔を取る

			if (!GetShipDataFromFile(coord, 0,
				&*ship, teamA[i].ship.ReferShipType(),
				count)) {//フラグシープのタイプの位置から譲る
				return false;
			}
			count++;
		}
	}

	coord = { 3000,-10,1800 };
	count = 11;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < teamB[i].ReferNumber(); j++) {

			enemyFleet.push_back(ShipMain());
			auto ship = enemyFleet.end();
			ship--;
			coord.x += 300;//戦艦の間の間隔を取る

			if (!GetShipDataFromFile(coord, MathAndPhysics::PI,
				&*ship, teamB[i].ship.ReferShipType(), 
				count)) {
				return false;
			}
			count++;
		}
	}
	return true;
}

bool IngameDataManagement::GetShipDataFromFile(Coordinate<double> Coord,
	double radian, ShipMain *ship, int shipType,int num) {
	ship->SetCoord(Coord);
	ship->SetRadianOnZ(radian);
	if (!ship->InifThisShip(&PL, ET, &SL, shipType,
		num)) {
		DrawString(10, 10, "ファイル読み込む失敗", GetColor(255, 255, 255));
		DxLib::ScreenFlip();
		WaitKey();
		return false;
	}
	return true;
}

void IngameDataManagement::DrawFormationBoard() {
	SetDrawScreen(DX_SCREEN_BACK);

	ClearDrawScreen();

	DrawExtendGraph(0, 0,
		Screen::SCREEN_X, Screen::SCREEN_Z,
		*formationBoard[FormationBoard::FB_BACKGROUND], FALSE);

	teamA[0].DrawCard(Coordinate2D<double>{70, 100});
	teamA[1].DrawCard(Coordinate2D<double>{70, 220});
	teamA[2].DrawCard(Coordinate2D<double>{70, 340});
	teamA[3].DrawCard(Coordinate2D<double>{70, 460});

	teamB[0].DrawCard(Coordinate2D<double>{710, 100});
	teamB[1].DrawCard(Coordinate2D<double>{710, 220});
	teamB[2].DrawCard(Coordinate2D<double>{710, 340});
	teamB[3].DrawCard(Coordinate2D<double>{710, 460});

	SetFontSize(35);
	DrawFormatString(300, 16, GetColor(255, 255, 255),
		"総数%2d隻(10隻まで)",teamACount);
	DrawFormatString(670, 16, GetColor(255, 255, 255),
		"総数%2d隻(10隻まで)", teamBCount);

	CUI.Draw();

	Coordinate2D<int> mousePos;

	GetMousePoint(&mousePos.x, &mousePos.z);

	for (int i = 0; i < 4; i++) {
		if (teamA[i].CheckMousePos({ 70,100 + 120 * i }, mousePos)) {
			teamA[i].DrawBar(mousePos);
		}
		if (teamB[i].CheckMousePos({ 710,100 + 120 * i }, mousePos)) {
			teamB[i].DrawBar(mousePos);
		}
	}

	DrawErrorList();

	DxLib::ScreenFlip();
}

bool IngameDataManagement::InifFormationBoard() {
	for (int i = 0; i < 4; i++) {
		teamA.push_back(ShipCard(PL.ReferFormationBoardHandle(FormationBoard::FB_SHIP_CARD),
			PL.ReferFantasyNumber()));
		teamB.push_back(ShipCard(PL.ReferFormationBoardHandle(FormationBoard::FB_SHIP_CARD),
			PL.ReferFantasyNumber()));
	}
	
	if (!teamA[0].ship.InifThisShip(&PL, ET, &SL, 4000, 1)) return false;
	if (!teamA[1].ship.InifThisShip(&PL, ET, &SL, 5000, 1)) return false;
	if (!teamA[2].ship.InifThisShip(&PL, ET, &SL, 4001, 1)) return false;
	if (!teamA[3].ship.InifThisShip(&PL, ET, &SL, 5001, 1)) return false;

	SetRandom(6, 0, true, &teamACount);//ランダムに生成する

	if (!teamB[0].ship.InifThisShip(&PL, ET, &SL, 4000, 1)) return false;
	if (!teamB[1].ship.InifThisShip(&PL, ET, &SL, 5000, 1)) return false;
	if (!teamB[2].ship.InifThisShip(&PL, ET, &SL, 4001, 1)) return false;
	if (!teamB[3].ship.InifThisShip(&PL, ET, &SL, 5001, 1)) return false;

	SetRandom(6, 0, false, &teamBCount);//ランダムに生成する

	teamA[flagShipNum].SetFlag();
	return true;
}

bool IngameDataManagement::FreeFormationBoard() {//フリーした後にメニューを通常状態に設置する
	
	if (ProcessMessage() == 0 && shouldIContinue)
		if (!RegisterTeam())
			return false;
	
	for (int i = 0; i < 4; i++) {
		teamA[i].ship.DestroyMemory();
		teamB[i].ship.DestroyMemory();
	}

	teamA.clear();
	teamB.clear();
	std::vector<ShipCard>().swap(teamA);
	std::vector<ShipCard>().swap(teamB);

	if (ProcessMessage() == 0 && shouldIContinue) {
		alliesFleet[0].SetControled();//友軍艦隊の一番の操作権を取る
		enemyFlagShip = &*enemyFleet.begin();
		CUI.InifShipList(&enemyFleet, false);
		CUI.InifShipList(&alliesFleet, true);
	}

	ClearErrorList();

	CUI.CloseFormationMenu();
	FC.Reset();

	return true;
}

void IngameDataManagement::DrawStatisticBoard2() {
	rewind(stdin);
	statisticBoardData.GetDis(alliesFleet[0].ReferDistanceMoved()*
		MathAndPhysics::Change_Distance/3600);
	statisticBoardData.Read(win);
	FC.Reset();
	while (ProcessMessage() == 0) {
		if (statisticBoardData.Update())
			break;
		FC.Wait();
	}
}

/****************************************************/
/*                     テスト　                     */
/****************************************************/
void IngameDataManagement::TEST_DRAW() {
	unsigned int Cr;
	Cr = GetColor(255, 255, 255);
	auto ship = alliesFleet.begin();
	//ship->TestDraw(MainCamera.ReferRealCameraX(),
	//	MainCamera.ReferRealCameraZ());
	char CharNum[255];

	DxLib::SetFontSize(15);
	ChangeFont("ＤＦＧ龍門石碑体");

	_gcvt_s(CharNum, ship->ReferSpeedOnZ() * 100, 10);
	DrawString(60, 10, "Speed", Cr);
	DrawString(110, 10, CharNum, Cr);
	if (ship->ReferReturnOn()) {
		DrawString(60, 30, "Alignment On", Cr);
	}
	else {
		DrawString(60, 30, "Alignment Off", Cr);
	}
	_gcvt_s(CharNum, ship->ReferChangingRadian(), 10);
	DrawString(60, 50, "Radian(C)", Cr);
	DrawString(150, 50, CharNum, Cr);
	_gcvt_s(CharNum, ship->ReferCoordX(), 10);
	DrawString(60, 70, "X", Cr);
	DrawString(80, 70, CharNum, Cr);
	_gcvt_s(CharNum, ship->ReferCoordZ(), 10);
	DrawString(60, 90, "Z", Cr);
	DrawString(80, 90, CharNum, Cr);
	_gcvt_s(CharNum, ship->ReferRadianOnZ(), 10);
	DrawString(60, 110, "Radian(R)", Cr);
	DrawString(150, 110, CharNum, Cr);
	_gcvt_s(CharNum, cos(ship->ReferRadianOnZ()), 10);
	DrawString(60, 130, "Cos", Cr);
	DrawString(100, 130, CharNum, Cr);
	_gcvt_s(CharNum, sin(ship->ReferRadianOnZ()), 10);
	DrawString(60, 150, "Sin", Cr);
	DrawString(100, 150, CharNum, Cr);
	_gcvt_s(CharNum, ship->ReferOutPutRate(), 10);
	DrawString(60, 170, "OutPutRate", Cr);
	DrawString(170, 170, CharNum, Cr);
	DrawString(60, 190, "FPS:", Cr);
	DrawFormatString(100, 190, Cr, "%2.3f", FC.ReferFPS());
}

void IngameDataManagement::EndTheGame() {
	showLock = false;
	if (sinkingShip.empty())
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
		(answer >= CommandSerial::SELECT && answer < CommandSerial::ZOOM_IN)||
		answer == CommandSerial::AUTO_FIRE)
		if (!CUI.ReferClickable())
			return;

	/*マウスでコマンドを受けなかった場合はキーボードからコマンドを受け取る*/
	if (answer == CommandSerial::NONE_COMMAND)
		answer = CT.GetCommand();

	/*コマンドがなければこの関数を終了する*/
	if (answer == CommandSerial::NONE_COMMAND)
		return;

	auto ship = alliesFleet.begin();

	/*船を操作*/
	if (ship->ReferAlive()) {
		ship->ControlThisShip(answer);
	}

	CUI.SetClickTime();//クリックした時間を記録する
	
	/*select以前のコマンドの処理*/
	if (answer < CommandSerial::SELECT) {
		switch (answer) {
		case CommandSerial::SHOOT:if(alliesFleet[0].ReferAlive()) 
			TestShoot(&alliesFleet[0],true); break;/*射撃*/
		case CommandSerial::MENU:CUI.LetMeSeeMenu();break;/*メニュー状態変更*/
		case CommandSerial::TEST_VIEW_ON:TEST_SHOW_ON = !TEST_SHOW_ON; break;	/*テストビュー*/
		case CommandSerial::EXIT:GameOver = true; break;	/*ゲーム終了*/
		case CommandSerial::CHANGE_CAMERA:showLock = !showLock; break;
		case CommandSerial::AUTO_FIRE:autoFire = !autoFire; break;
		}
	}

	else if (answer >= CommandSerial::ZOOM_IN) {
		switch (answer) {
		case CommandSerial::ZOOM_IN:MainCamera.ZoomIn(); break;
		case CommandSerial::ZOOM_OUT:MainCamera.ZoomOut(); break;
		case CommandSerial::GET_DAMAGE_TEST:alliesFleet[0].SufferDamage(5);
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
			autoFire = false;//自動射撃を無効化
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
		if(ship->ReferAlive())
		ship->Move();//友軍の船を移動する
	}
	if(!enemyFleet.empty())//リスト確認
	for (auto ship = enemyFleet.begin();
		ship != enemyFleet.end();
		ship++) {
		if (ship->ReferAlive())
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
bool IngameDataManagement::Inif() {

	long long startTime = GetTickCount();

	SetUseASyncLoadFlag(TRUE);//非同期読み込みを有効化
	PL.InifForGame();//画像ローダー初期化
	SL.Inif();//音声ローダー初期化
	SetUseASyncLoadFlag(FALSE);//非同期読み込みを無効化

	SetDrawScreen(DX_SCREEN_BACK);

	asyncLoadNum = GetASyncLoadNum();

	while (ProcessMessage() == 0) {
		DrawLoading(asyncLoadNum - GetASyncLoadNum());
		if(GetASyncLoadNum() ==0)
			break;
		if (GetTickCount() - startTime > TIME_NEEDED::ONE_MINUTE ) {
			return false;
		}
	}

	Sleep(10);
	SetFontThickness(6);
	PL.GetGraphSizeForGame();//画像の大きさを設置する
	UI.InifUI(&PL);//UI初期化
	ET.InifEffectTemplate(&PL);//エフェクトテンプレート初期化
	CT.Inif(&SL);//キーボードコントローラー初期化
	CUI.IngameInif(&PL,&SL);//マウスコントローラー初期化
	
	statisticBoardData.Inif(PL.ReferStatisticBoardHandle(0));/*統計ボードーの初期化*/
	if (!InifFormationBoard()) {//編成画面を初期化する
		DrawString(30, 30, "ファイル読み込む失敗\nタイトルに戻ります",
			GetColor(255, 255, 255));
		DxLib::ScreenFlip();
		WaitKey();
		return false;
	}

	for (int i = 0; i < FormationBoard::FB_NUM; i++) {
		formationBoard[i] = PL.ReferFormationBoardHandle(i);
	}

	CUI.SetFormationMenuStatus();/*ここはテストバージョン*/

	return true;
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
	normalEffectList.clear();
	msgList.clear();
}

/*制限時間超えたものを消す*/
void IngameDataManagement::DeleteUseless() {
	CheckErrorList();
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

void IngameDataManagement::DestroyThisTeam(std::list<ShipMain> *shipList) {
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
	if (alliesFleet[0].ReferAlive()) {//フラグシープが沈むまで戦える
		for (auto ship = alliesFleet.begin();
			ship != alliesFleet.end();
			ship++) {
			if (ship->ReferAlive())
				return;
		}
	}
	win = false;//友軍艦隊は全滅されたら負けです
	GameEnd = true;/*終わる状態を設置*/
}

void IngameDataManagement::CheckEnemyStatus() {
	if (enemyFleet[0].ReferAlive())
		for (auto ship = enemyFleet.begin();
			ship != enemyFleet.end();
			ship++) {
		if (ship->ReferAlive())
			return;
	}
	win = true;//敵軍艦隊は全滅されたら負けです
	GameEnd = true;/*終わる状態を設置*/
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
			if (abs(ship->ReferSpeedOnZ()) > 0.05 && rand() % 30 == 0
				&& rand() % 100 > ship->ReferSpeedOnZ() * 10) {//一定の速度があれば、確率で生成する
				//for(int i = 0; i < ship->ReferBubbleCount(); i++)
//				if (counter % 20 == 0)
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
					&& counter % 10 == 0) {//確率で生成する
					//for(int i = 0; i < ship->ReferSmokeCount(); i++)
					for (int i = 0; i < ship->ReferSmokePointCount(); i++)
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
	Effect effect(TypeOfEffect::EXPLOSION,
		false, 500, radian, 0, 0, 0, Coord.x, Coord.z,
		PL.ReferEffectList(TypeOfEffect::EXPLOSION), true,
		0.15 * ((double)(rand()%4) + 1), 1.05);

	explosionList.push_back(effect);//生成されたものをリストの後ろに追加する
}

void IngameDataManagement::NewRipple(double coordX, double coordZ) {
	Effect ripple(TypeOfEffect::RIPPLE,
		false, 600, 0, 0, 0, 0, coordX, coordZ,
		PL.ReferEffectList(TypeOfEffect::RIPPLE), true, 0.45, 1.09);
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

	shouldIRender = counter % 60 == 0 ? true : false;//毎秒一回レンダーする

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
				ship->LockAndAim(&enemyList[ship->fireDataFigureUp.ReferTarget()],
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
					statisticBoardData.CountShoot();
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
				if (ship2 -> ReferAlive())//生きる状態確認
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
		if (ship2->ReferSinkingEnding())//船にの状態を確認
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
		if (shell->ReferUsable())
			if (shell->ReferSerialNumber() != ship->ReferSerialNumber()) {
				//弾と船のあたり判定を行う
				if (crash3DtoPoint(ship->ReferCoord(),
					shell->ReferCoord(), ship->ReferShipCrashSize(),
					ship->ReferRadianOnZ()))
					if (ship->ReferAlive()) {
						//当たったら
						ship->SufferDamage((int)shell->ReferDamage());//ダメージを与える
						Coordinate2D<double> C2D = { shell->ReferCoordX(),
						shell->ReferCoordZ() };
						NewExplosion(C2D);//当たったところに爆発エフェクトを生成
						shell->Unusable();//弾が使えなくなる

						if (shell->ReferSerialNumber() == 0) {
							statisticBoardData.CountHit();//ヒット数増加
							statisticBoardData.CountDamage((int)shell->ReferDamage());//ダメージ数増加
							if (!ship->ReferAlive()) {
								statisticBoardData.CountKilled();
							}
						}
						if (ship->ReferSerialNumber() == 0)
							statisticBoardData.CountDamageRec((int)shell->ReferDamage());

						/*ロックを使えない時にロックを解除する*/
						if (!ship->ReferAlive())//沈む状態判定
							if (alliesFleet[0].fireDataFigureUp.ReferTarget() ==
								//ロックのターゲットではないか
								ship->ReferSerialNumber() - SELECT_RANGE - 1 &&
								//敵ですか
								ship->ReferSerialNumber() > SELECT_RANGE) {
								if (alliesFleet[0].fireDataFigureUp.ReferLockOn() == true) {
									alliesFleet[0].fireDataFigureUp.LockOn_Switch();//ロック状態を変更
									ship->ResetReviseData();//修正データをリセット	
									CUI.SetShootMenu(false);//ＵＩを変更
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
			if (!ship->ReferAlive()&&!ship->ReferInList()) {
				/*ここはアドレスをリストに追加したみたい。。。*/
				sinkingShip.push_back(*ship);//沈んでいるリストに追加する
				ship->PutInList();
			}
		}
	}
}

void IngameDataManagement::RemoveSinkedShip() {
	if (!sinkingShip.empty()) {
		for (auto ship = sinkingShip.begin();
			ship != sinkingShip.end();
			ship++) {
			if (ship->ReferSinkingEnding()) {
				/*独立なメモリを使っていません、destroyMemoryはいらない*/
				ship = sinkingShip.erase(ship);
			}
			if (sinkingShip.empty() || ship == sinkingShip.end())
				break;
		}
	}
}

/*エラー処理*/
void IngameDataManagement::DrawErrorList() {
	if (!msgList.empty())
		for (auto msg = msgList.begin();
			msg != msgList.end();
			msg++) {
		msg->DrawMessege();
	}
}

void IngameDataManagement::CheckErrorList() {
	if(!msgList.empty())
	for (auto msg = msgList.begin();
		msg != msgList.end();
		msg++) {
		if (msg->ReferDestroy()) {
			msg = msgList.erase(msg);
			if (msgList.empty() || msg == msgList.end())
				break;
		}
	}
}

void IngameDataManagement::ClearErrorList() {
	msgList.empty();
}

void IngameDataManagement::SetRandom(int left, int num, bool teamA, int *teamCount) {
	int temp = 0;
	temp = rand() % left + 1;
	if (temp > maxCountInATeam / 3) {//一種類の船の数を多すぎにならないように
		temp /= 2;
	}
	if (left <= 2)//最後残った部分全部あげる
		temp = left;
	SetThisShipCount(teamCount, num, temp, teamA);
	left -= temp;
	if (left <= 0 || num + 1 >= 4)
		return;
	SetRandom(left, num + 1, teamA, teamCount);
}

void IngameDataManagement::ClearTeam() {
	for (int i = 0; i < 4; i++) {
		teamA[i].SetNumber(0);
		teamB[i].SetNumber(0);
	}
	teamACount = 0;
	teamBCount = 0;
}

bool IngameDataManagement::ShouldIDraw() {
	if (shouldIDraw)
		return true;
	if (!shouldIDraw && haveIDrawed) {
		haveIDrawed = false;
		return true;
	}

	if (!shouldIDraw) {
		haveIDrawed = true;
		return false;
	}
	return false;
}