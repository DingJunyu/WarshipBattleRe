#pragma once
#include"DxLib.h"
#include"DefinedData.h"
class Button {
public:
	Button() {
		pressed = false;
		zoomMultiple = 1.0f;
	}
	~Button();

	void Inif(int *bH, int *bPH);
	void SetXZ(double xP, double zP, double zM);
	void CheckXZ();

	void DrawThisButton();
	void CheckMousePosition(int mouseX, int mouseZ);
	void ChangePressed() { pressed = !pressed; }

	int ReferGraphSizeX() { return graphSizeX; }
	int ReferGraphSizeZ() { return graphSizeZ; }

private:
	/*À•W*/
	int coordX;
	int coordZ;

	/*•`‰æ—p•Ï”*/
	double xPercent;
	double zPercent;
	
	int *buttonHandle;/*•’Ê•`‰æ*/
	int *buttonPressedHandle;/*‰Ÿ‚³‚ê‚½‚Ì•`‰æ*/

	int graphSizeX;
	int graphSizeZ;

	double zoomMultiple;/*‰æ‘œ‚Ì”{—¦*/

	bool pressed;//ƒ}ƒEƒX‚ª”ÍˆÍ“à‚É“ü‚Á‚½‚©
};