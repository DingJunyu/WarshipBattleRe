#pragma once
#include"PictureLoader.h"
#include"DxLib.h"
#include"SoundLoader.h"
#include"DefinedData.h"
#include"ButtonCollection.h"
#include"OtherFunctions.h"
#include"ColorBox.h"
#include"FrameControl.h"
/*���Ƃ��Ƃ͂��̃N���X�Ń��C�����j���[�̑�����������邾�����A
���ۂɂق��̉�ʂ̃R���g���[���[�̓��e���d�����܂����A
���ꂩ�烆�[�U�[�̑���͂��ׂĂق��̃N���X�Ɉڍs���܂��B*/
class MainMenuController
{
public:
	MainMenuController() {
		countForTitle = 0;
		firstTimeGetIntoMainMenu = true;
		titleMultipleRateInTitle = 2;
		titleMultipleRateInMainMenu = 4;
		buttonMultiple = 0.7;
		buttonMultiple_TTR = 0.35;
		TTR_choice = TutorialNum::DIRECT_CONTROL;
		MoviePlaying = -1;
		Loading = true;
	}
	~MainMenuController();

	void Inif();
	void Inif_Tur();

	void DrawTitle();//�^�C�g����ʂ�`�悷��
	void DrawMainMenu();//���C�����j���[��`�悷��
	void DrawLoading();//Loading
	void DrawTutorial();

	int CheckChoice();//�I�������m�F
	void FREE();

	void SetTutorialChoice(int x) { TTR_choice = x; }

private:
	void StandardInif();

	void SetButtonPosition();
	void SetButtonPos_Tur();

	void DrawTutorialSingle();

	void LoadMovie();
	void DeleteMovie();
	void CheckPlayStatus();
	void PlayVideoOrDrawBack(int x,int z, double rate);

	/*���ۂɉ摜�ۑ��N���X�͂����ɐ錾���ׂ��ł͂Ȃ��������A
	�f�U�C���̃~�X�ł����ɐ錾���܂����A���̃N���X�̖������ԈႢ�܂����A
	MainMenuCOntroller�ł͂Ȃ��AClickableUserInterface�ɒ����ׂ���*/
	/*���߂�ClickableUserInterface�̃N���X�����܂��B*/
	PictureLoader PL;
	SoundLoader SL;
	ButtonCollection BC;
	FrameControl FC;
	ColorBox CrBox;

	/*�񎦂̓_�ŗp�J�E���g*/
	int countForTitle;

	/*�{�^���̕���*/
	enum ButtonCoord
	{
		AMOUNT = 3,
		BC_X = 0,
		BC_Z,
		BC_ACTIVE //������true��false���g��
	};
	double buttonPosition[ButtonEvent::BUTTON_AMOUNT][ButtonCoord::AMOUNT];
	int buttonSizeX;
	int buttonSizeZ;
	double buttonMultiple;
	double buttonMultiple_TTR;

	/*�w�i*/
	int *backGroundHandle;
	/*�^�C�g��*/
	int *title;
	int titleSizeX;
	int titleSizeZ;

	/*�^�C�g���̑傫���̓^�C�g���̒��ƃ��C�����j���[�̒����Ⴄ*/
	int titleMultipleRateInTitle;
	int titleMultipleRateInMainMenu;

	/*�{�^��������*/
	int *pressedToStart;
	int PTSSizeX;
	int PTSSizeZ;
	int *pressedToStartDown;

	int titleMultiple;

	/*���߂ă��C�����j���[���Ăяo�����ɏ����������s��*/
	bool firstTimeGetIntoMainMenu;

	/*�`���[�g���A��*/
	int *tutorialHandle;

	int TTR_choice;

	int MoviePlaying;
	int VideoHandle;
	int v_Size_X, v_Size_Z;
	bool Loading;

	bool PlayMovie;

	int asyncLoadNum;//�񓯊��ǂݍ��ސ�
	LONGLONG startTime;
};

