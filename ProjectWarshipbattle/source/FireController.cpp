#include "FireController.h"

FireController::FireController():testAmmo(true,false,true,true)
{
}

FireController::~FireController()
{
}

void FireController::CalTheAnswer() {
	while (testAmmo.ReferCoordY() > 0) {
		testAmmo.Move();//�C�ʂɗ�����܂ňړ�����
	}
	answer = testAmmo.ReferCoord();//�������W���L�^����
}