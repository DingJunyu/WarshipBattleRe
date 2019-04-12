#pragma once
#include"DefinedData.h"
#include"ButtonCollection.h"
#include"PictureLoader.h"
#include"DxLib.h"
#include"SoundLoader.h"
#include<cmath>
/*今回のゲームでは、すべての操作をマウスでできるようにしたいです。
メインメニューにも、ゲーム画面にも使えるクラスです。
目標としては同じ構造を利用して、
インターフェースを変更すれば違う中身を利用できるようにしたいです*/
/*音声ローダー、画像ローダーはすべて外部から取りますので、再宣言することを控えます*/
/*戻り値などはすべてDefinedData.hに定義します。*/
/*なお、直接触れない部分はUserInterfaceクラスに実現します。
このクラスでは、クリックできるパーツの描画と命令を返すの二つの機能を実現します。*/

class ClickableUserInterface
{
public:
	ClickableUserInterface() {
		menuOpened = false;
		lastClickTime = GetTickCount();
	}
	~ClickableUserInterface();

	void IngameInif(PictureLoader *PL, SoundLoader *SL);
	
	/*マウスの座標を取ってコマンドを取る*/
	int CheckChoice();
	void Draw();
	/*最初に現るものを見えるように設定する*/
	void SetNormalStatus();
	/*メニューが使えるかのスイッチ*/
	void LetMeSeeMenu();
	void CloseMenu();
	bool CheckMenuOpened() { return menuOpened; }

	/*押された時間を記録*/
	void SetClickTime() { lastClickTime = GetTickCount(); }

	/*連続押しができない時、押すことが有効であるかを判断する*/
	bool ReferClickable() {
		if (lastClickTime + inputDelay <= GetTickCount())
			return true;
		return false;
	}

	/*メモリ解放*/
	void Free();

private:
	/*数の変更もできるようにしました*/
	void MemorySecure(int);
	void MemoryFree(int);
	void SetThisOne(int num, double x, double z, double mt);

	/*提示の点滅用カウント*/
	int countForTitle;

	bool menuOpened;
	unsigned int lastClickTime;
	const int inputDelay = 150;

	ButtonCollection BC;

	/*SaveForCuiに参考してください*/
	/*ボタンのデータを保存している*/
	double **buttonPosition;
};