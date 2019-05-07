#pragma once
#include"PictureLoader.h"
#include"DxLib.h"
#include"SoundLoader.h"
#include"DefinedData.h"
#include"ButtonCollection.h"
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
	}
	~MainMenuController();

	void Inif();

	void DrawTitle();//�^�C�g����ʂ�`�悷��
	void DrawMainMenu();//���C�����j���[��`�悷��
	void DrawLoading();//Loading

	int CheckChoice();//�I�������m�F
	void FREE();

private:
	void SetButtonPosition();

	/*���ۂɉ摜�ۑ��N���X�͂����ɐ錾���ׂ��ł͂Ȃ��������A
	�f�U�C���̃~�X�ł����ɐ錾���܂����A���̃N���X�̖������ԈႢ�܂����A
	MainMenuCOntroller�ł͂Ȃ��AClickableUserInterface�ɒ����ׂ���*/
	/*���߂�ClickableUserInterface�̃N���X�����܂��B*/
	PictureLoader PL;
	SoundLoader SL;
	ButtonCollection BC;
	FrameControl FC;

	/*�񎦂̓_�ŗp�J�E���g*/
	int countForTitle;

	/*�{�^���̕���*/
	enum ButtonCoord
	{
		AMOUNT = 2,
		X = 0,
		Z
	};
	double buttonPosition[ButtonEvent::BUTTON_AMOUNT][ButtonCoord::AMOUNT];
	int buttonSizeX;
	int buttonSizeZ;
	double buttonMultiple;

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
};

