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
	/*���W*/
	int coordX;
	int coordZ;

	/*�`��p�ϐ�*/
	double xPercent;
	double zPercent;
	
	int *buttonHandle;/*���ʕ`��*/
	int *buttonPressedHandle;/*�����ꂽ���̕`��*/

	int graphSizeX;
	int graphSizeZ;

	double zoomMultiple;/*�摜�̔{��*/

	bool pressed;//�}�E�X���͈͓��ɓ�������
};