/****************************************************************/
/*このゲームはDxLibを利用して開発しました                       */
/*https://dxlib.xsrv.jp/                                        */
/*ゲーム情報:                                                   */
/*作者:丁 俊宇                                                  */
/*学校:大原情報ビジネス専門学校                                 */
/*学科:ゲーム制作コース                                         */
/*戦艦画像はwikiに載せたものです                                */
/*アップロード者:https://commons.wikimedia.org/wiki/User:Alexpl */
/*UI作者:丁俊宇                                                 */
/*                                                              */
/*更新日:2019.5.15                                             */
/****************************************************************/
#include"DxLib.h"
#include"HeaderForMain.h"

void GameMain();
void SingleGame_Mission_Progress();
void SingleGame_DeathMatch_Progress();
int MainMenu();

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);//ウインドモードに変更
	SetWaitVSyncFlag(FALSE);//自動同期を無効にする
	SetMainWindowText("WarshipBattle");
	SetGraphMode(Screen::SCREEN_X, Screen::SCREEN_Z, Screen::COLOR);
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}
	/*ここにはウインドーズメッセージの問題がある、
	DXLIBの場合は名前を変えれば解決できるが、
	WINAPIの場合はほかの処理をしないといけない*/

	SetAlwaysRunFlag(TRUE);
	srand((unsigned)time(NULL));//乱数の初期化

	MainMenuController MMC;
	int choice = -1;//選択を無効にする
	bool endGame = false;//ゲームオーバーフラグをオフにする

	while (!endGame) {

		choice = MainMenu();

		if (ProcessMessage() == -1)
			break;

		//メニューから取ったコマンドに合わせて関数を呼び出す
		switch (choice) {
		/*まだロード機能実装しなかったため、いずれのボタンを押したらメインゲームが始まる*/
		case ButtonEvent::NEW_GAME:
		case ButtonEvent::CONTINUE_GAME :
		case ButtonEvent::LOAD_GAME:
			SingleGame_DeathMatch_Progress(); break;	
		case ButtonEvent::GAME_OVER:endGame = true; break;
		}
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}

int MainMenu(){
	MainMenuController MMC;
	int choice = -1;

		MMC.Inif();//メニュー部分初期化
		MMC.DrawTitle();//タイトル描く
		MMC.DrawLoading();
		choice = -1;//選択を初期化

		while (choice == -1) {
			MMC.DrawMainMenu();//メインメニューを描く
			choice = MMC.CheckChoice();//選択を確認・バー状態更新
									   //メインメニューから脱出
			if (CheckHitKey(KEY_INPUT_ESCAPE) == TRUE &&
				GetInputChar(TRUE)) {
				choice = ButtonEvent::GAME_OVER;
			}
			if (ProcessMessage() == -1)
				break;
		}

		MMC.DrawLoading();//この関数はほかの部分に移動する
		MMC.FREE();//メモリを解放
		return choice;
}

void SingleGame_DeathMatch_Progress() {
	
	//ゲーム管理の部品を宣言
	IngameDataManagement IDM;
	TeamDeathMatchControl TDMC;

	IDM.Inif();//初期化
	IDM.FormationBoard();//編成画面を描く
	IDM.FreeFormationBoard();//編成に使ったメモリなどを解放する

	/*ゲームメインプログレス*/
	if (ProcessMessage() == 0) {
		while (!TDMC.GameOver(IDM.TeamDestroyed())) {//一つチームが全滅まで繰り返す
			IDM.Update();
			if (ProcessMessage() == -1)//エラー処理
				break;
			if (IDM.ReferEndGame())//他の原因でゲームを終了したっら
				break;
		}
		IDM.DrawLoading();//ロード画面を描く
		/*結果発表*/
		IDM.DrawStatisticBoard();//統計画面を描く
		/*メモリ解放*/
	}
	IDM.Free();
}