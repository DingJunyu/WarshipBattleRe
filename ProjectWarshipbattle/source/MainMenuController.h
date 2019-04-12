#pragma once
#include"PictureLoader.h"
#include"DxLib.h"
#include"SoundLoader.h"
#include"DefinedData.h"
#include"ButtonCollection.h"
#include"FrameControl.h"
/*もともとはこのクラスでメインメニューの操作を実現するだけが、
実際にほかの画面のコントローラーの内容が重複しました、
これからユーザーの操作はすべてほかのクラスに移行します。*/
class MainMenuController
{
public:
	MainMenuController() {
		countForTitle = 0;
		firstTimeGetIntoMainMenu = true;
		titleMultipleRateInTitle = 2;
		titleMultipleRateInMainMenu = 4;
		buttonMultiple = 0.5;
	}
	~MainMenuController();

	void Inif();

	void DrawTitle();
	void DrawMainMenu();
	void DrawLoading();

	int CheckChoice();
	void FREE();

private:
	void SetButtonPosition();

	/*実際に画像保存クラスはここに宣言すべきではなかったが、
	デザインのミスでここに宣言しました、このクラスの命名も間違いました、
	MainMenuCOntrollerではなく、ClickableUserInterfaceに直すべきだ*/
	/*改めてClickableUserInterfaceのクラスを作ります。*/
	PictureLoader PL;
	SoundLoader SL;
	ButtonCollection BC;
	FrameControl FC;

	/*提示の点滅用カウント*/
	int countForTitle;

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

	int *backGroundHandle;
	int *title;
	int titleSizeX;
	int titleSizeZ;

	int titleMultipleRateInTitle;
	int titleMultipleRateInMainMenu;

	int *pressedToStart;
	int PTSSizeX;
	int PTSSizeZ;
	int *pressedToStartDown;

	int titleMultiple;

	bool firstTimeGetIntoMainMenu;
};

