#pragma once
#include"PictureLoader.h"
#include"DxLib.h"
#include"SoundLoader.h"
#include"DefinedData.h"
#include"ButtonCollection.h"
#include"OtherFunctions.h"
#include"ColorBox.h"
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
		buttonMultiple = 0.7;
		buttonMultiple_TTR = 0.35;
		TTR_choice = TutorialNum::DIRECT_CONTROL;
		MoviePlaying = -1;
		Loading = true;
	}
	~MainMenuController();

	void Inif();
	void Inif_Tur();

	void DrawTitle();//タイトル画面を描画する
	void DrawMainMenu();//メインメニューを描画する
	void DrawLoading();//Loading
	void DrawTutorial();

	int CheckChoice();//選択肢を確認
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

	/*実際に画像保存クラスはここに宣言すべきではなかったが、
	デザインのミスでここに宣言しました、このクラスの命名も間違いました、
	MainMenuCOntrollerではなく、ClickableUserInterfaceに直すべきだ*/
	/*改めてClickableUserInterfaceのクラスを作ります。*/
	PictureLoader PL;
	SoundLoader SL;
	ButtonCollection BC;
	FrameControl FC;
	ColorBox CrBox;

	/*提示の点滅用カウント*/
	int countForTitle;

	/*ボタンの部分*/
	enum ButtonCoord
	{
		AMOUNT = 3,
		BC_X = 0,
		BC_Z,
		BC_ACTIVE //ここはtrueとfalseを使う
	};
	double buttonPosition[ButtonEvent::BUTTON_AMOUNT][ButtonCoord::AMOUNT];
	int buttonSizeX;
	int buttonSizeZ;
	double buttonMultiple;
	double buttonMultiple_TTR;

	/*背景*/
	int *backGroundHandle;
	/*タイトル*/
	int *title;
	int titleSizeX;
	int titleSizeZ;

	/*タイトルの大きさはタイトルの中とメインメニューの中が違う*/
	int titleMultipleRateInTitle;
	int titleMultipleRateInMainMenu;

	/*ボタンを押す*/
	int *pressedToStart;
	int PTSSizeX;
	int PTSSizeZ;
	int *pressedToStartDown;

	int titleMultiple;

	/*初めてメインメニューを呼び出す時に初期処理を行う*/
	bool firstTimeGetIntoMainMenu;

	/*チュートリアル*/
	int *tutorialHandle;

	int TTR_choice;

	int MoviePlaying;
	int VideoHandle;
	int v_Size_X, v_Size_Z;
	bool Loading;

	bool PlayMovie;

	int asyncLoadNum;//非同期読み込む数
	LONGLONG startTime;
};

