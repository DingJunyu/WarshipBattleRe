#include "FireController.h"

FireController::FireController():testAmmo(true,false,true,true)
{
}

FireController::~FireController()
{
}

void FireController::CalTheAnswer() {
	while (testAmmo.ReferCoordY() > 0) {
		testAmmo.Move();//海面に落ちるまで移動する
	}
	answer = testAmmo.ReferCoord();//落下座標を記録する
}