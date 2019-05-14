#include "EffectTemplate.h"

EffectTemplate::EffectTemplate(){
}

EffectTemplate::~EffectTemplate(){
}

void EffectTemplate::InifEffectTemplate(PictureLoader *PL) {
	InifCreateBubble(PL);//画像ローダーを利用して初期化する
	InifCreateSmoke(PL);//画像ローダーを利用して初期化する
}

void EffectTemplate::InifCreateBubble(PictureLoader *PL) {
	createBubble.Inif(TypeOfEffect::BUBBLE,PL->ReferEffectList(TypeOfEffect::BUBBLE));
}

void EffectTemplate::InifCreateSmoke(PictureLoader *PL) {
	createSmoke.Inif(TypeOfEffect::SMOKE, PL->ReferEffectList(TypeOfEffect::SMOKE));
}