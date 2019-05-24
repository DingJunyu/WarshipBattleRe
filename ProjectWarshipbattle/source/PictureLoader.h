#pragma once
#include"DxLib.h"
#include"DefinedData.h"
/*���ׂẲ����f�[�^���Ǘ�����N���X*/
class PictureLoader
{
public:
	PictureLoader() {}
	~PictureLoader();

	/*�������֐�*/
	void AllInif() { MemorySecure(); LoadPicture(); }
	/*�������֐�*/
	void MemorySecure();
	void LoadPicture();

	/*���������*/
	void FreeMemory();
	void FreePicture();

	/*���C�����j���[�p�C���^�[�t�F�[�X*/
	void InifForMainMenu() {
		MemorySecure();
		LoadMainMenu();
		LoadButtonInMainMenu();
	}
	/*�Q�[���p�C���^�[�t�F�[�X*/
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

	/*���������*/
	/*�K���Ō�Ɏg���I�I�I*/
	void FREE_ALL() { FreePicture(); FreeMemory(); }
	void FREE_FOR_MAINMENU() {
		DeleteMainMenu();
		DeleteButtonInMainMenu();
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

	//�₢���킹�֐�
	//�D
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

	//�}�b�v
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

	//�G�t�F�N�g
	int *ReferEffectList(int num) { return &effectHandleList[num][0]; }

	//�{�^��
	int *ReferButtonHandle(int num) { return &buttonHandle[num]; }
	int *ReferButtonPressedHandle(int num) {
		return &buttonPressedHandle[num];}

	int *ReferButtonIntheGame(int num) { return &buttonIntheGame[num]; }
	int *ReferButtonPressedIntheGame(int num){
		return &buttonPressedIntheGame[num];}

	//�^�C�g���֌W
	int *ReferTitle() { return &title; }
	int *ReferPressedToStart() { return &pressedToStart; }
	int *ReferPressedToStartDown() { return &pressedToStartDown; }
	int *ReferBackGround() { return &backGround; }

	//�e
	int *ReferAmmoHandle(int x) { return &ammoHandle[x]; }

	int *ReferFantasyNumber() { return &fantasyNumbers[0]; }

private:
	

	/*�@�\�ʃ��[�h�֐�*/
	/*2019.4.9�ǂݍ��ݕ����𕪉����āA�g���������������[�h����悤�ɂ������ł�*/
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

	/*�@�\�ʃ���������֐�*/
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

	/*�D�摜�f�[�^*/
	/*�Q�l�FDefinedData.h-SerialNumberOfShipHandle*/
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

	/*�}�b�v�I�u�W�F�N�g�f�[�^*/
	int **mapObjectHandle;

	/*�}�b�v�f�[�^*/
	/*�n���h��*/
	int mapHandle;
	int noiseHandle;
	/*�T�C�Y*/
	int mapX;
	int mapZ;

	/*�e�f�[�^*/
	/*�n���h��*/
	int *ammoHandle;
	/*�T�C�Y*/
	int *ammoX;
	int *ammoZ;

	/*UI�f�[�^*/
	/*UI�x�[�X*/
	int matHandle;
	int matHandleX;
	int matHandleZ;
	int shipCardMatHandle;
	int shipCardMatHandleX;
	int shipCardMatHandleZ;
	int miniMapMatHandle;
	int miniMapPaperHandle;

	int UIList[UI_LIST::AMOUNT];//���ׂĂ̂t�h�\�[�X��ۑ�����

	/*�̓}�[�N*/
	int **markForAllies;
	int **markForEnemies;

	/*�G�t�F�N�g*/
	int effectHandleList[TypeOfEffect::EFFECT_AMOUNT][TypeOfEffect::EFFECT_FLAME_AMOUNT];

	//�{�^��
	int *buttonHandle;
	int *buttonPressedHandle;

	int buttonIntheGame[CommandSerial::COUNT];
	int buttonPressedIntheGame[CommandSerial::COUNT];

	//�^�C�g���֌W
	int title;
	int pressedToStart;
	int pressedToStartDown;
	int backGround;

	int statisticBoardHandle[StatisticBoard::SB_NUM];
	int formationBoardHandle[FormationBoard::FB_NUM];

	int fantasyNumbers[11];
};