#include "Morale.h"
/*士気クラス*/

Morale::Morale()
{
	morale = 100;
	movable = true;
	mainPartDamageCount = 0;
	subPartDamageCount = 0;
}


Morale::~Morale()
{
}

/*今の士気を確認して返す*/
void Morale::CheckMorale() {
	if (mainPartDamageCount % MORALE_DOWN_AFTER_MAINPARTDAMAGECOUNT ==
		MORALE_DOWN_AFTER_MAINPARTDAMAGECOUNT - 1) {
		morale -= MORALE_DOWN_WITH_MAINPARTDAMAGE;
		mainPartDamageCount = 0;
	}
	if (subPartDamageCount%MORALE_DOWN_AFTER_SUBPARTDAMAGECOUNT ==
		MORALE_DOWN_AFTER_MAINPARTDAMAGECOUNT - 1) {
		morale -= MORALE_DOWN_WITH_SUBPARTDAMAGE;
		subPartDamageCount = 0;
	}
	if (morale <= 0) {
		movable = false;
	}
}