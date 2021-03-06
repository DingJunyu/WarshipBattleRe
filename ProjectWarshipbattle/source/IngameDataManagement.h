#pragma once
#include"HeaderForIngameDataManagement.h"
/*関数のプライベート化はこれからする*/
class IngameDataManagement
{
public:
	IngameDataManagement() :MainCamera(CameraType::MAIN) {
		TEST_SHOW_ON = false;
		GameOver = false;
		GameEnd = false;
		autoFire = false;
		counter = 0;
		shouldIRender = false;
		showLock = false;
		shouldIContinue = true;
		
		shouldIDraw = true;
		haveIDrawed = false;

		flameCount = 0;
		flagShipNum = 0;
		multipleRate = 0.5;
	}
	~IngameDataManagement();

	//メインループ
	//ゲーム本体
	void Update();

	//ゲームを初期化操作
	//ここの関数にファイル操作も含む
	bool Inif();
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

	//ロード画面
	void DrawLoading();
	void DrawLoading(int num);

	bool FormationBoard();
	bool RegisterTeam();
	bool GetShipDataFromFile(Coordinate<double> Coord,double radian,ShipMain *ship,
		int shipType, int num);
	bool FreeFormationBoard();

	void DrawStatisticBoard2();
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
	StatisticBoardData statisticBoardData;

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
	int counter;//60までカウントする
	double flameCount;
	bool shouldIRender;//探索状況更新スイッチ
	bool showLock;

	//描く関数
	void DrawAll();
	void TEST_DRAW();//デバッグ用データ確認
	void DrawSea();//海を描く//九枚を描くため、効率が低いになって、もう使わない
	void DrawSea_New();//新しい四枚描くバージョン
	void DrawSea_New_Extend();
	void DrawMesh_Sea();
	void DrawMesh_Sea_Extend();
	void DrawShips();//船を描く
	void DrawShipsShadow();//影を描く
	void DrawShipsOnMiniMap();//ミニマップを描く
	void DrawPlanes();
	void DrawAmmo();//弾を描く
	void DrawBomb();
	void DrawTorpedo();
	void DrawThisList(std::list<Effect> *effectList);//一種類のエフェクトを描く
	void DrawEffectUnderShips();//船の下にあるエフェクトを描く
	void DrawEffectBeyondShips();//船の上にあるエフェクトを描く
	void DrawPointOfImpact();
	void DrawHPBar();
	void DrawBarForATeam(std::vector<ShipMain> shipList, bool left);

	//移動関数
	void MoveAll();
	void MoveShips();
	void MoveEffect(std::list<Effect> *effectList);
	void MoveEffects();
	void MoveAmmo();
	void CheckShipListStatus(std::vector<ShipMain> *shipList);
	void CheckShipsStatus();//船の状態を更新する
	void SinkingListUpdate();
	void SinkingShipUpdate(ShipMain *ship);

	//コントロール関数
	void Control();

	//テスト関連
	bool TEST_SHOW_ON;//デバッグ画面を起用するか
	void SIMPLE_USER_INTERFACE();//ステータスを文字にして表示する
	bool GameOver;//ゲーム終了マーク
	bool GameEnd;

	//あたり判定など
	void CrashDecision();//あたり判定-船と船の間
	bool PointsToCollisionbox(ShipMain *ship1,ShipMain *ship2);
	void HitDecision();//ゲームコントローラーに当たる数を返す
	void SimpleHitDecision();
	void CheckThisTeamDecision(std::vector<ShipMain> *shipList, Ammo *shell);//このチームと弾リストの間にあたり判定を行う
	void RemoveDestroyedShip();//沈んだ船を消す
	void CheckTeamA(std::vector<ShipMain> *teashipListm);//沈んだ船を沈む演出リストに追加する
	void RemoveSinkedShip();

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
	bool autoFire;//プレーヤー自動射撃ボタン
	bool shouldIContinue;
	int flagShipNum;
	void DeleteUseless();
	void DestroyShips();
	void DestroyThisTeam(std::vector<ShipMain> *shipList);
	void DestroyThisTeam(std::list<ShipMain> *shipList);
	void CheckTeamStatus();
	void CheckAlliesStatus();
	void CheckEnemyStatus();
	void EndTheGame();

	void DrawFormationBoard();
	bool InifFormationBoard();

	/*エラー処理*/
	void DrawErrorList();
	void CheckErrorList();
	void ClearErrorList();

	std::vector<ShipMain> alliesFleet;//自軍艦隊
	std::vector<ShipMain> enemyFleet;//敵軍艦隊
	ArtificialIntelligence AI;
	std::list<ShipMain> sinkingShip;//沈んでいる船

	ShipMain *enemyFlagShip;

	std::list<Ammo> shellList;//砲弾リスト
	std::list<Bomb> bombList;//爆弾リスト
	std::list<Torpedo> torpedoList;//魚雷リスト
	std::list<Effect> bubbleList;//水泡リスト
	std::list<Effect> smokeList;//雲リスト
	std::list<Effect> explosionList;//爆発リスト
	std::list<Effect> rippleList;//水泡リスト(砲弾落下地)
	std::list<Effect> normalEffectList;
	std::list<ErrorMessege> msgList;
	
	/*編成*/
	int *formationBoard[FormationBoard::FB_NUM];
	std::vector<ShipCard> teamA;
	std::vector<ShipCard> teamB;

	const int maxCountInATeam = 10;
	int teamACount = 0;
	int teamBCount = 0;
	bool SetThisShipCount(int *Count,int number,int Amount,bool TeamA) {
		if (*Count + Amount <= maxCountInATeam) {
			(*Count) += Amount;
			if (TeamA) {
				teamA[number].SetNumber(Amount);
			}
			else {
				teamB[number].SetNumber(Amount);
			}
			return true;
		}
		return false;//変更失敗時にファルスを返す
	}

	void SetRandom(int left, int num, bool teamA, int *teamCount);
	void ClearTeam();

	int asyncLoadNum;//非同期読み込む数
	LONGLONG startTime;

	double multipleRate;

	/*描画コントロール*/
	bool shouldIDraw;
	bool haveIDrawed;

	bool ShouldIDraw();
};