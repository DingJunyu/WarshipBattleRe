#pragma once
#include"DxLib.h"
#include"DefinedData.h"
/*クリックできるボタンの動作を実現するためのクラス*/
/*メニュークラスとＵＩ部分に使わる*/
class Button {
public:
	Button() {
		pressed = false;
		zoomMultiple = 1.0f;
	}
	~Button();

	/*初期化関数と設置関数*/
	void Inif(int *bH, int *bPH);
	void SetXZ(double xP, double zP, double zM);
	void CheckXZ();

	/*処理関数*/
	void DrawThisButton();
	void CheckMousePosition(int mouseX, int mouseZ);
	void ChangePressed() { pressed = !pressed; }

	/*問い合わせ関数*/
	int ReferGraphSizeX() { return graphSizeX; }
	int ReferGraphSizeZ() { return graphSizeZ; }
	double ReferZoomMultiple() { return zoomMultiple; }
	Coordinate2D<int> ReferCoord() { return Coordinate2D<int> {coordX, coordZ}; }

private:
	/*座標*/
	int coordX;
	int coordZ;

	/*描画用変数*/
	double xPercent;
	double zPercent;
	
	int *buttonHandle;/*普通描画*/
	int *buttonPressedHandle;/*押された時の描画*/

	int graphSizeX;
	int graphSizeZ;

	double zoomMultiple;/*画像の倍率*/

	bool pressed;//マウスが範囲内に入ったか
};