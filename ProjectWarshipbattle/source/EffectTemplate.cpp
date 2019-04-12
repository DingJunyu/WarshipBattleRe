#include "EffectTemplate.h"

EffectTemplate::EffectTemplate(){
}

EffectTemplate::~EffectTemplate(){
}

void EffectTemplate::InifEffectTemplate(PictureLoader *PL) {
	InifCreateBubble(PL);
	InifCreateSmoke(PL);
}

void EffectTemplate::InifCreateBubble(PictureLoader *PL) {
	createBubble.Inif(TypeOfEffect::BUBBLE,PL->ReferBubbleHandle());
}

void EffectTemplate::InifCreateSmoke(PictureLoader *PL) {
	createSmoke.Inif(TypeOfEffect::SMOKE, PL->ReferSmokeHandle());
}