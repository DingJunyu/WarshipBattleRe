#include "Button.h"

Button::~Button() {
}

void Button::Inif(int *bH, int *bPH) {
	/*画像データを読み込む*/
	buttonHandle = bH;
	buttonPressedHandle = bPH;
	/*画像のサイズを取る*/
	GetGraphSize(*bH, &graphSizeX, &graphSizeZ);
	/*押されたではない*/
	pressed = false;
}

void Button::SetXZ(double xP, double zP, double zM) {
	xPercent = xP;
	zPercent = zP;

	/*ヘッダーファイルからデータを読み込む*/
	coordX = (int)(xP * Screen::SCREEN_X);
	coordZ = (int)(zP * Screen::SCREEN_Z);

	zoomMultiple = zM;//拡大率を設定する
}

void Button::CheckXZ() {
	coordX = (int)(xPercent * Screen::SCREEN_X);
	coordZ = (int)(zPercent * Screen::SCREEN_Z);
}

void Button::CheckMousePosition(int mouseX, int mouseZ) {
	/*もしマウスが範囲内に入ったら、pressedを有効にする*/
	if (mouseX > coordX && mouseX < coordX + graphSizeX * zoomMultiple) {
		if (mouseZ > coordZ && mouseZ < coordZ + graphSizeZ * zoomMultiple) {
			pressed = true;
		}
	}
	pressed = false;//押された状態を解除する
}

void Button::DrawThisButton() {
	if (!pressed)//押されなかった場合
		DrawExtendGraph((int)coordX, (int)coordZ,
			(int)(coordX + graphSizeX * zoomMultiple),
			(int)(coordZ + graphSizeZ * zoomMultiple), 
			*buttonHandle, TRUE);
	else//押された場合
		DrawExtendGraph((int)coordX, (int)coordZ,
			(int)(coordX + graphSizeX * zoomMultiple),
			(int)(coordZ + graphSizeZ * zoomMultiple),
			*buttonPressedHandle, TRUE);
	//描画後、押された状態を解除
	pressed = false;
}