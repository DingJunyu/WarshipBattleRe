#include "Button.h"

Button::~Button() {
}

void Button::Inif(int *bH, int *bPH) {
	/*�摜�f�[�^��ǂݍ���*/
	buttonHandle = bH;
	buttonPressedHandle = bPH;
	/*�摜�̃T�C�Y�����*/
	GetGraphSize(*bH, &graphSizeX, &graphSizeZ);
	/*�����ꂽ�ł͂Ȃ�*/
	pressed = false;
}

void Button::SetXZ(double xP, double zP, double zM) {
	xPercent = xP;
	zPercent = zP;

	/*�w�b�_�[�t�@�C������f�[�^��ǂݍ���*/
	coordX = (int)(xP * Screen::SCREEN_X);
	coordZ = (int)(zP * Screen::SCREEN_Z);

	zoomMultiple = zM;//�g�嗦��ݒ肷��
}

void Button::CheckXZ() {
	coordX = (int)(xPercent * Screen::SCREEN_X);
	coordZ = (int)(zPercent * Screen::SCREEN_Z);
}

void Button::CheckMousePosition(int mouseX, int mouseZ) {
	/*�����}�E�X���͈͓��ɓ�������Apressed��L���ɂ���*/
	if (mouseX > coordX && mouseX < coordX + graphSizeX * zoomMultiple) {
		if (mouseZ > coordZ && mouseZ < coordZ + graphSizeZ * zoomMultiple) {
			pressed = true;
		}
	}
	pressed = false;//�����ꂽ��Ԃ���������
}

void Button::DrawThisButton() {
	if (!pressed)//������Ȃ������ꍇ
		DrawExtendGraph((int)coordX, (int)coordZ,
			(int)(coordX + graphSizeX * zoomMultiple),
			(int)(coordZ + graphSizeZ * zoomMultiple), 
			*buttonHandle, TRUE);
	else//�����ꂽ�ꍇ
		DrawExtendGraph((int)coordX, (int)coordZ,
			(int)(coordX + graphSizeX * zoomMultiple),
			(int)(coordZ + graphSizeZ * zoomMultiple),
			*buttonPressedHandle, TRUE);
	//�`���A�����ꂽ��Ԃ�����
	pressed = false;
}