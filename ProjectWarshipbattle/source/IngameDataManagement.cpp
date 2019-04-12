#include "IngameDataManagement.h"
/*イテレータを使う前に必ず容器の状況を確認します！！！！！*/
using namespace DxLib;
IngameDataManagement::~IngameDataManagement()
{
}

/*メインルート*/
void IngameDataManagement::Update() {
	if (FC.Wait()) {
		CUI.SetNormalStatus();/*ここはテストバージョン*/

		MainCamera.GetXZ(ReferPlayerX(), ReferPlayerZ());//カメラ座標を更新
		Control();//コマンドを受け取って、船の状態を変更する
		GetNewEffect();//エフェクトを生成する
		MoveAll();//船の状態による座標を変更する
		SimpleHitDecision();
		//	CrashDecision();//船の間のあたり判定
		//	HitDecision();//砲弾と船の間のあたり判定
		DeleteUseless();//入らないものを消す
		CheckAndPlaySound();
		DrawAll();//全部更新した後画面を描く
	}
}

/*ゲームコントロール*/
bool IngameDataManagement::TeamDestroyed() {
	if (alliesFleet.empty() || enemyFleet.empty())
		return false;
	return false;
}

/****************************************************/
/*                     描画関連                     */
/****************************************************/
void IngameDataManagement::DrawAll() {
	SetDrawScreen(DX_SCREEN_BACK);//裏画面に描画する
	ClearDrawScreen();

	/*海を描く*/
	DrawSea();

	/*移動できるパーツを描く*/
	DrawEffectUnderShips();
//	DrawTorpedo();
	DrawShips();
	DrawEffectBeyondShips();
//	DrawPlanes();
	DrawAmmo();
//	DrawBomb();

	/*UI関連*/
	auto ship = alliesFleet.begin();//イテレータで操作している船のステータスを取る
	UI.DrawUI();//普通のUIを描画する
	UI.DrawUINeedInput(&*ship);//船のステータスに関係があるUIを描画する
	DrawShipsOnMiniMap();//ミニマップを描く
	CUI.Draw();//ボタンを描く

	/*テストインフォメーション*/
	if (TEST_SHOW_ON)
		TEST_DRAW();
	
	ScreenFlip();
}

void IngameDataManagement::DrawShips() {
	//友軍を描画する 
	//コントロールしている船が友軍艦隊の一番にする
	if (!alliesFleet.empty())
		for (auto mark = alliesFleet.begin();
			mark != alliesFleet.end(); mark++) {
		if (mark->ReferAlive()) {
			if (mark == alliesFleet.begin())
				mark->Draw(MainCamera);//自分だけが画面中心に映す
			else
				mark->DrawSub(MainCamera);//それ以外のは相対座標を利用して描く
		}
	}
	if(!enemyFleet.empty())
		for (auto ship = enemyFleet.begin();
			ship != enemyFleet.end(); ship++) {
		if (ship->ReferAlive()) {
			ship->DrawSub(MainCamera);//敵軍は全部相対座標を利用して描く
		}
	}
}

void IngameDataManagement::DrawShipsOnMiniMap() {
	//友軍艦隊
	if(!alliesFleet.empty())
	for (auto ship = alliesFleet.begin();
		ship != alliesFleet.end(); ship++) {
		if (ship->ReferAlive()) {
			UI.DrawShipOnTheMap(ship->ReferCoordX(),
				ship->ReferCoordZ(), false);//青いマーク
		}
	}
	if (!enemyFleet.empty())
		for (auto ship = enemyFleet.begin();
			ship != enemyFleet.end(); ship++) {
		if (ship->ReferAlive()) {
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
	if (!effectList->empty())
		for (auto effect = effectList->begin();
			effect != effectList->end();
			effect++) {
		effect->Draw((int)MainCamera.ReferRealCameraX(),
			(int)MainCamera.ReferRealCameraZ());//相対座標を利用して描く
	}
}

/*船の下にあるエフェクトを描く*/
void IngameDataManagement::DrawEffectUnderShips() {
	/*水泡演出*/
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 90);//透明度を下がる
	DrawThisList(&bubbleList);
	DrawThisList(&rippleList);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);//描画モードをもとに戻る
}

/*船の上にあるエフェクトを描く*/
void IngameDataManagement::DrawEffectBeyondShips() {
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 240);//透明度を下がる
	DrawThisList(&smokeList);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140);//透明度を下がる
	DrawThisList(&explosionList);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);//描画モードをもとに戻る
}

void IngameDataManagement::DrawAmmo() {
	if (!shellList.empty())
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
	SetFontSize(30);

	DrawString((int)(0.85*Screen::SCREEN_X),
		(int)(0.85*Screen::SCREEN_Z), "LOADING...", Cr);

	ScreenFlip();
}

/****************************************************/
/*                     テスト　                     */
/****************************************************/
void IngameDataManagement::TEST() {
	alliesFleet.push_back(ShipMain());//新しい船を生成する
	auto ship = alliesFleet.begin();//イテレータを使って自分の船を選ぶ
	//ここの部分は初期化関数とファイル読み込みはまだ出来ていない
	//初期化関数は下のようにしたい
	ship->InifThisShip(PL.ReferBattleCrusierHandle(4000), 
		PL.ReferBattleCrusierShadowHandle(4000), 4000, ET, &SL);//初期化
	ship->NewCoordX(2200);//新しい座標をあげる
	ship->NewCoordZ(1500);
	ship->NewCoordY(0);
	ship->SetRadianOnZ(0);
	ship->SetLength(PL.ReferShipSizeX());//サイズを設定
	ship->SetWidth(PL.ReferShipSizeZ());
	ship->TEST();//テスト関数をロードする
	ship->SetWeaponTest(&PL);//武器をロードする


	for (int i = 0; i < 15; i++) {
		enemyFleet.push_back(ShipMain());//テスト用敵船を生成する
		auto enemyShip = enemyFleet.end();//イテレータで船を選ぶ
		enemyShip--;
		double radian = (double)(rand() % 180) / 180.0*MathAndPhysics::PI;

		enemyShip->InifThisShip(PL.ReferBattleCrusierHandle(4000),
			PL.ReferBattleCrusierShadowHandle(4000), 4000, ET, &SL);
		enemyShip->NewCoordX(2600 + i * (rand() % 400));
		enemyShip->NewCoordZ(1350 + i * (rand() % 400));
		enemyShip->NewCoordY(0);
		enemyShip->SetRadianOnZ(radian);
		enemyShip->SetLength(PL.ReferShipSizeX());
		enemyShip->SetWidth(PL.ReferShipSizeZ());
		enemyShip->TEST();
		enemyShip->SetSerialNumber(2);
		enemyShip->SetWeaponTest(&PL);
	}
}

void IngameDataManagement::TEST_DRAW() {
	unsigned int Cr;
	Cr = GetColor(255, 255, 255);
	auto ship = alliesFleet.begin();
	ship->TestDraw(MainCamera.ReferRealCameraX(),
		MainCamera.ReferRealCameraZ());
	char CharNum[255];

	SetFontSize(15);
	ChangeFont("HGｺﾞﾐｯｸM");

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
		answer == CommandSerial::TURN_RETURN)
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
	

	/*射撃*/
	if (answer == CommandSerial::SHOOT) {
		TestShoot();
	}

	/*UI関連*/
	if (answer == CommandSerial::MENU) {
		if (!CUI.CheckMenuOpened()) {
			CUI.LetMeSeeMenu();//メニューを開く
		}
		else {
			CUI.CloseMenu();//メニューを閉じる
		}
	}

	/*テストビュー*/
	if (answer == CommandSerial::TEST_VIEW_ON)
		TEST_SHOW_ON = !TEST_SHOW_ON;

	/*ゲーム終了*/
	if (answer == CommandSerial::EXIT) {
		GameOver = true;
	}

	CUI.SetClickTime();
}

/****************************************************/
/*                     移動関連                     */
/****************************************************/
void IngameDataManagement::MoveAll() {
	CheckShipsStatus();//船の状態を更新する
	MoveShips();//船を移動する
	MoveEffects();//エフェクトを移動する
	MoveAmmo();//弾を移動する
}

void IngameDataManagement::CheckShipListStatus(std::vector<ShipMain> *shipList) {
	if (!shipList->empty())
		for (auto ship = shipList->begin();
			ship != shipList->end();
			ship++) {
		ship->CalSpeed();//速度更新
		ship->Alignment();//舵はもとに戻る
	}
}

void IngameDataManagement::CheckShipsStatus() {
	CheckShipListStatus(&alliesFleet);
	CheckShipListStatus(&enemyFleet);
}

void IngameDataManagement::MoveShips() {
	if(!alliesFleet.empty())
	for (auto ship = alliesFleet.begin();
		ship != alliesFleet.end();
		ship++) {
		ship->Move();//友軍の船を移動する
	}
	if(!enemyFleet.empty())
	for (auto ship = enemyFleet.begin();
		ship != enemyFleet.end();
		ship++) {
		ship->Move();//敵の船を移動する
	}
}

void IngameDataManagement::MoveEffect(std::list<Effect> *effectList) {
	if (!effectList->empty())
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
	if (!shellList.empty())
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
}

/*使ったメモリを解放する*/
void IngameDataManagement::Free() {
	PL.FREE_FOR_GAME();
	SL.FreeAll();
	CUI.Free();
}

/*制限時間超えたものを消す*/
void IngameDataManagement::DeleteUseless() {
	DeleteUselessEffect();//時間切りエフェクトを消す
	DeleteUselessAmmo();//海に落ちた砲弾を消す
	RemoveDestroyedShip();
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
	if (!alliesFleet.empty()) {
		for (auto ship = alliesFleet.begin();
			ship != alliesFleet.end();
			ship++) {
			if (abs(ship->ReferSpeedOnZ()) > 0.1 && rand() % 3 == 0
				&& rand() % 100 > ship->ReferSpeedOnZ() * 10) {//一定の速度があれば、確率で生成する
				//for(int i = 0; i < ship->ReferBubbleCount(); i++)
				for (int i = 0; i < 4; i++)//すべてのエフェクトポイントからエフェクトを生成する
					bubbleList.push_back(ship->NewBubble(i));//リストの末に追加する
			}
		}
		for (auto ship = alliesFleet.begin();
			ship != alliesFleet.end();
			ship++) {
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

void IngameDataManagement::NewExplosion(Coordinate2D<double> Coord) {
	double radian = (double)(rand() % 180) / 180.0*MathAndPhysics::PI;
	Effect effect(false, 500, radian, 0, 0, 0, Coord.x, Coord.z,
		PL.ReferEffectList(TypeOfEffect::EXPLOSION), true, 0.08, 1.05);

	explosionList.push_back(effect);
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

void IngameDataManagement::TestShoot() {
	FiringData FD;
	//ゲーム中では、敵が射撃する時に全部trueにする
	FD.isThisMainWeapon = true;
	for (int i = 0; i < 8; i++) {
		FD.selected[i] = true;//武器を選ぶ
	}
	
	auto ship = alliesFleet.begin();
	InputNewAmmo(&*ship, FD);//リストに新しい弾を追加する
}

void IngameDataManagement::InputNewAmmo(ShipMain *SM, FiringData FD) {
	int weaponAmount = SM->ReferWeaponCount(FD.isThisMainWeapon);//選んだ武器を確認する
	for (int i = 0; i < weaponAmount; i++) {
		if (FD.selected[i]) {
			if (SM->IsThisOneUsable(i, FD.isThisMainWeapon)) {//射撃が可能であれば
				shellList.push_back(SM->Shoot(i,FD.isThisMainWeapon));//新しい弾を追加する
			}
		}
	}
}

void IngameDataManagement::DeleteUselessAmmo() {
	if (!shellList.empty())
		for (auto shell = shellList.begin();
			shell != shellList.end();
			shell++) {
		if (shell->FallIntoWater()|| !shell->ReferUsable()) {
			if (shell->FallIntoWater())
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
void IngameDataManagement::SimpleHitDecision() {
	if (!shellList.empty())
		for (auto shell = shellList.begin();
			shell != shellList.end();
			shell++) {
		CheckThisTeamDecision(&alliesFleet, &*shell);//友軍のチェック
		CheckThisTeamDecision(&enemyFleet, &*shell);//敵のチェック
	}
}

void IngameDataManagement::CheckThisTeamDecision(std::vector<ShipMain> *shipList,
	Ammo *shell) {
	if (!shipList->empty())
		for (auto ship = shipList->begin();
			ship != shipList->end();
			ship++) {
		if (shell->ReferSerialNumber() != ship->ReferSerialNumber()) {
			if (crash3DtoPoint(ship->ReferCoord(),
				shell->ReferCoord(),ship->ReferShipCrashSize(),
				ship->ReferRadianOnZ()))
			if(ship->ReferAlive()){
				//当たったらほかの関数を呼び出す
				//未完成
				ship->SufferDamage(shell->ReferDamage());
				Coordinate2D<double> C2D = { shell->ReferCoordX(),
				shell->ReferCoordZ() };
				NewExplosion(C2D);
				shell->Unusable();
				return;
			}
		}
	}
}

void IngameDataManagement::RemoveDestroyedShip() {
	CheckTeamA(&alliesFleet);
	CheckTeamA(&enemyFleet);
}

void IngameDataManagement::CheckTeamA(std::vector<ShipMain> *shipList) {
	if(!shipList->empty()){
		for (auto ship = shipList->begin();
			ship != shipList->end();
			ship++) {
			if (!ship->ReferAlive()) {
				sinkingShip.push_back(*ship);
				ship = shipList->erase(ship);
			}
			if (shipList->empty() || ship == shipList->end())
				return;
		}
	}
}