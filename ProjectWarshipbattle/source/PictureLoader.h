#pragma once
#include"DxLib.h"
#include"DefinedData.h"
/*すべての音声データを管理するクラス*/
class PictureLoader
{
public:
	PictureLoader() {}
	~PictureLoader();

	/*初期化関数*/
	void AllInif() { MemorySecure(); LoadPicture(); }
	/*初期化関数*/
	void MemorySecure();
	void LoadPicture();

	/*メモリ回収*/
	void FreeMemory();
	void FreePicture();

	/*メインメニュー用インターフェース*/
	void InifForMainMenu() {
		MemorySecure();
		LoadMainMenu();
		LoadButtonInMainMenu();
		LoadTutorial();
	}
	/*ゲーム用インターフェース*/
	void InifForGame() {
		MemorySecure();
		LoadShips();
		LoadSea();
		LoadUserInterfaceInGame();
		LoadEffect();
		LoadAmmo();
		LoadButtonInGame();
		LoadStatisticBoard();
		LoadFormationBoard();
		LoadFantasyNumbers();
	}

	/*メモリ回収*/
	/*必ず最後に使う！！！*/
	void FREE_ALL() { FreePicture(); FreeMemory(); }
	void FREE_FOR_MAINMENU() {
		DeleteMainMenu();
		DeleteButtonInMainMenu();
		DeleteTutorial();
		FreeMemory();
	}
	void FREE_FOR_GAME(){
		DeleteShips();
		DeleteSea();
		DeleteUserInterfaceInGame();
		DeleteEffect();
		DeleteAmmo();
		DeleteButtonInGame();
		DeleteStatisticBoard();
		DeleteFormationBoard();
		DeleteFantasyNumbers();
		FreeMemory();
	}
	void GetGraphSizeForGame();

	//問い合わせ関数
	//船
	int ReferShipSizeX() { return shipSizeX; }
	int ReferShipSizeZ() { return shipSizeZ; }

	int *ReferShipHandle(int x) {
		return &shipHandle[x]
		[SerialNumberOfShipHandle::SHIP_HANDLE];
	}

	int *ReferShipShadowHandle(int x) {
		return &shipHandle[x]
		[SerialNumberOfShipHandle::SHIP_SHADOW_HANDLE];
	}

	//マップ
	int ReferMapX() { return mapX; }
	int ReferMapZ() { return mapZ; }

	int *ReferMapHandle() { return &mapHandle; }
	int *ReferNoiseHandle() { return &noiseHandle; }

	int *ReferStatisticBoardHandle(int x) { return &statisticBoardHandle[x]; }
	int *ReferFormationBoardHandle(int x) { return &formationBoardHandle[x]; }

	//UI
	int *ReferMatHandle() { return &matHandle; }
	int ReferMatHandleX() { return matHandleX; }
	int ReferMatHandleZ() { return matHandleZ; }

	int *ReferShipCardMatHandle() { return &shipCardMatHandle; }
	int ReferShipCardMatHandleX() { return shipCardMatHandleX; }
	int ReferShipCardMatHandleZ() { return shipCardMatHandleZ; }

	int *ReferMiniMapMatHandle() { return &miniMapMatHandle; }
	int *ReferMiniMapPaperHandle() { return &miniMapPaperHandle; }

	int *ReferUIList(int x) { return &UIList[x]; }

	//エフェクト
	int *ReferEffectList(int num) { return &effectHandleList[num][0]; }

	//ボタン
	int *ReferButtonHandle(int num) { return &buttonHandle[num]; }
	int *ReferButtonPressedHandle(int num) {
		return &buttonPressedHandle[num];}

	int *ReferButtonIntheGame(int num) { return &buttonIntheGame[num]; }
	int *ReferButtonPressedIntheGame(int num){
		return &buttonPressedIntheGame[num];}

	//タイトル関係
	int *ReferTitle() { return &title; }
	int *ReferPressedToStart() { return &pressedToStart; }
	int *ReferPressedToStartDown() { return &pressedToStartDown; }
	int *ReferBackGround() { return &backGround; }

	//弾
	int *ReferAmmoHandle(int x) { return &ammoHandle[x]; }

	int *ReferFantasyNumber() { return &fantasyNumbers[0]; }

	/*チュートリアル*/
	int *ReferTutorialHandle() { return &tutorialHandle[0]; }

private:
	

	/*機能別ロード関数*/
	/*2019.4.9読み込み部分を分解して、使う部分だけをロードするようにしたいです*/
	void LoadShips();
	void LoadSea();
	void LoadUserInterfaceInGame();
	void LoadEffect();
	void LoadAmmo();
	void LoadMainMenu();
	void LoadButtonInMainMenu();
	void LoadButtonInGame();
	void LoadStatisticBoard();
	void LoadFormationBoard();
	void LoadFantasyNumbers();
	void LoadTutorial();

	/*機能別メモリ解放関数*/
	void DeleteShips();
	void DeleteSea();
	void DeleteUserInterfaceInGame();
	void DeleteEffect();
	void DeleteAmmo();
	void DeleteMainMenu();
	void DeleteButtonInMainMenu();
	void DeleteButtonInGame();
	void DeleteStatisticBoard();
	void DeleteFormationBoard();
	void DeleteFantasyNumbers();
	void DeleteTutorial();

	/*船画像データ*/
	/*参考：DefinedData.h-SerialNumberOfShipHandle*/
	int **destroyerHandle;
	int **lightCruiserHandle;
	int **heaveyCruiserHandle;
	int **shipHandle;
	int **battleShipHandle;
	int **aircraftCarrierHandle;
	int **lightAircraftCarrierHandle;
	int **CVEHandle;
	int shipSizeX;
	int shipSizeZ;

	/*マップオブジェクトデータ*/
	int **mapObjectHandle;

	/*マップデータ*/
	/*ハンドル*/
	int mapHandle;
	int noiseHandle;
	/*サイズ*/
	int mapX;
	int mapZ;

	/*弾データ*/
	/*ハンドル*/
	int *ammoHandle;
	/*サイズ*/
	int *ammoX;
	int *ammoZ;

	/*UIデータ*/
	/*UIベース*/
	int matHandle;
	int matHandleX;
	int matHandleZ;
	int shipCardMatHandle;
	int shipCardMatHandleX;
	int shipCardMatHandleZ;
	int miniMapMatHandle;
	int miniMapPaperHandle;

	int UIList[UI_LIST::AMOUNT];//すべてのＵＩソースを保存する

	/*チュートリアル*/
	int tutorialHandle[TutorialNum::TN_AMOUNT];

	/*艦マーク*/
	int **markForAllies;
	int **markForEnemies;

	/*エフェクト*/
	int effectHandleList[TypeOfEffect::EFFECT_AMOUNT][TypeOfEffect::EFFECT_FLAME_AMOUNT];

	//ボタン
	int *buttonHandle;
	int *buttonPressedHandle;

	int buttonIntheGame[CommandSerial::COUNT];
	int buttonPressedIntheGame[CommandSerial::COUNT];

	//タイトル関係
	int title;
	int pressedToStart;
	int pressedToStartDown;
	int backGround;

	int statisticBoardHandle[StatisticBoard::SB_NUM];
	int formationBoardHandle[FormationBoard::FB_NUM];

	int fantasyNumbers[11];
};