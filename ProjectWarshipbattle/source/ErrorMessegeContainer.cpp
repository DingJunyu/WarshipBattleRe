#include "ErrorMessegeContainer.h"

ErrorMessege Fleet_Empty() {
	return ErrorMessege("��͐�����Ȃ�(���Ȃ��Ƃ���`�[���͈�䂪�K�v��",
		{ 50,650 }, { 600,680 });
}

ErrorMessege FlagShip_Empty() {
	return ErrorMessege("�t���O�V�[�v���K�v��",
		{ 50,650 }, { 350,680 });
}

ErrorMessege PressToContinue() {
	return ErrorMessege("Press AnyKey to Continue",
		{ 400,650 }, { 700,680 });
}