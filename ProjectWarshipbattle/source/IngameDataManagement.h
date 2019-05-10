#pragma once
#include"HeaderForIngameDataManagement.h"
/*関数のプライベート化はこれからする*/
class IngameDataManagement
{
public:
	IngameDataManagement() :MainCamera(CameraType::MAIN) {
		TEST_SHOW_ON = false;
		GameOver = false;
		counter = 0;
		shouldIRender = false;
		showLock = false;
	}
	~IngameDataManagement();

	//メインループ
	//ゲーム本体
	void Update();

	//ゲームを初期化操作
	//ここの関数にファイル操作も含む
	void Inif();
	void registerTeam();
	void Free();

	//カメラ用座標問い合わせ
	double ReferPlayerX() { return alliesFleet[0].ReferCoordX(); }
	double ReferPlayerZ() { return alliesFleet[0].ReferCoordZ(); }
	double ReferTargetX(int x) { return enemyFleet[x].ReferCoordX(); }
	double ReferTargetZ(int x) { return enemyFleet[x].ReferCoordZ(); }
	
	//GameController問い合わせ
	int ReferRemainedAlliesNum();
	int ReferRemainedEnemiesNum();
	int ReferRemainedAlliesNumInHanger();
	int ReferRemainedAlliesNumInSky();
	int ReferRemainedEnemiesNumInHanger();
	int ReferRemainedEnemiesNumInSky();

	bool TeamDestroyed();
	bool ReferEndGame() { return GameOver; }

	//統計データ
	int ShootCount();//実現はあとでいい

	//すべてのテストはここで行う
	void TEST();

	//ロード画面
	void DrawLoading();


	void FormationBoard();
	void FreeFormationBoard();

	void DrawStatisticBoard();
private:
	//他のコントローラー
	PictureLoader PL;//画像ローダー
	FrameControl FC;//フレームコントローラー
	Camera MainCamera;//メインカメラ
	Controller CT;//キーボード入力管理
	UserInterface UI;//クリックできないUI
	EffectTemplate ET;//エフェクトテンプレート
	SoundLoader SL;//音声ローダー
	ClickableUserInterface CUI;//クリックできるUI

	/*AI管理*/
	FlagShipAI flagShipAI;

	void AIUpdate();
	void LetFlagShipMove();
	void LetEveryOneMove();
	void ControlThisListMove(std::vector<ShipMain> *shipList,
		ArtificialIntelligence *AI);
	void LetEveryOneLockOn();
	void ControlThisListLock(std::vector<ShipMain> *shipList,
		std::vector<ShipMain> enemyList);
	void LetEveryOneShoot();
	void ControlThisListShoot(std::vector<ShipMain> *shipList);

	/*描画処理関連*/
	long long counter;//60までカウントする
	bool shouldIRender;//探索状況更新スイッチ
	bool showLock;

	//描く関数
	void DrawAll();
	void TEST_DRAW();
	void DrawSea();
	void DrawShips();
	void DrawShipsShadow();
	void DrawShipsOnMiniMap();
	void DrawPlanes();
	void DrawAmmo();
	void DrawBomb();
	void DrawTorpedo();
	void DrawThisList(std::list<Effect> *effectList);
	void DrawEffectUnderShips();
	void DrawEffectBeyondShips();
	void DrawPointOfImpact();

	//移動関数
	void MoveAll();
	void MoveShips();
	void MoveEffect(std::list<Effect> *effectList);
	void MoveEffects();
	void MoveAmmo();
	void CheckShipListStatus(std::vector<ShipMain> *shipList);
	void CheckShipsStatus();//船の状態を更新する

	//コントロール関数
	void Control();

	//テスト関連
	bool TEST_SHOW_ON;//デバッグ画面を起用するか
	void SIMPLE_USER_INTERFACE();//ステータスを文字にして表示する
	bool GameOver;//ゲーム終了マーク

	//あたり判定など
	void CrashDecision();//あたり判定-船と船の間
	bool PointsToCollisionbox(ShipMain *ship1,ShipMain *ship2);
	void HitDecision();//ゲームコントローラーに当たる数を返す
	void SimpleHitDecision();
	void CheckThisTeamDecision(std::vector<ShipMain> *shipList, Ammo *shell);//このチームと弾リストの間にあたり判定を行う
	void RemoveDestroyedShip();//沈んだ船を消す
	void CheckTeamA(std::vector<ShipMain> *teashipListm);//沈んだ船を沈む演出リストに追加する

	//弾管理
	void TestShoot(ShipMain *ship,bool me);

	void LockAndRefresh();
	void CheckThisTeamLock(std::vector<ShipMain> *shipList,
		std::vector<ShipMain> enemyList);

	void CheckSelectedWeapon();
	void InputNewAmmo(ShipMain *SM, FiringData FD,bool me);//新しい弾をlistに追加
	void DeleteUselessAmmo();//期限切りものを削除

	/*音声関数*/
	void CheckAndPlaySound();

	/*エフェクト関連*/
	void GetNewEffect();
	void DeleteUselessEffect();
	void CheckAndCleanThisEffectList(std::list<Effect> *effectList);
	void NewEffectForShips(std::vector<ShipMain> shipList);
	void NewExplosion(Coordinate2D<double> Coord);
	void NewRipple(double coordX,double coordZ);//砲弾が海に落ちる時新しいエフェクトを生成

	/*ゲームコントロール*/
	bool win;
	void DeleteUseless();
	void DestroyShips();
	void DestroyThisTeam(std::vector<ShipMain> *shipList);
	void CheckTeamStatus();
	void CheckAlliesStatus();
	void CheckEnemyStatus();
	void TEST_WIN();

	void DrawFormationBoard();
	void InifFormationBoard();

	std::vector<ShipMain> alliesFleet;//自軍艦隊
	std::vector<ShipMain> enemyFleet;//敵軍艦隊
	ArtificialIntelligence AI;
	std::list<ShipMain> sinkingShip;//沈んでいる船

	std::list<Ammo> shellList;//砲弾リスト
	std::list<Bomb> bombList;//爆弾リスト
	std::list<Torpedo> torpedoList;//魚雷リスト
	std::list<Effect> bubbleList;//水泡リスト
	std::list<Effect> smokeList;//雲リスト
	std::list<Effect> explosionList;//爆発リスト
	std::list<Effect> rippleList;//水泡リスト(砲弾落下地)

	/*統計*/
	void InifStatisticBoardData() {
		shootCount = 0;
		hitCount = 0;
		movedDis = 0;
		damage = 0; damageRecieved = 0;
		killed = 0;
	}
	int *statisticBoard[StatisticBoard::SB_NUM];
	int shootCount;
	int hitCount;
	double hitRate;
	double movedDis;
	int damage;
	int damageRecieved;
	int killed;
	
	/*編成*/
	int *formationBoard[FormationBoard::FB_NUM];
	std::vector<ShipCard> teamA;
	std::vector<ShipCard> teamB;

	const int maxCountInATeam = 10;
	int teamAChoose[4][2];
	int teamBChoose[4][2];

};