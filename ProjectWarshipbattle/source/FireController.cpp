#include "FireController.h"

FireController::FireController():testAmmo(true,false,true,true)
{
}

FireController::~FireController()
{
}

void FireController::CalTheAnswer() {
	while (testAmmo.ReferCoordY() > 0) {
		testAmmo.Move();//ŠC–Ê‚É—‚¿‚é‚Ü‚ÅˆÚ“®‚·‚é
	}
	answer = testAmmo.ReferCoord();//—‰ºÀ•W‚ğ‹L˜^‚·‚é
}