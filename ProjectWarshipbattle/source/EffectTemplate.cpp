#include "EffectTemplate.h"

EffectTemplate::EffectTemplate(){
}

EffectTemplate::~EffectTemplate(){
}

void EffectTemplate::InifEffectTemplate(PictureLoader *PL) {
	InifCreateBubble(PL);//�摜���[�_�[�𗘗p���ď���������
	InifCreateSmoke(PL);//�摜���[�_�[�𗘗p���ď���������
}

void EffectTemplate::InifCreateBubble(PictureLoader *PL) {
	createBubble.Inif(TypeOfEffect::BUBBLE,PL->ReferEffectList(TypeOfEffect::BUBBLE));
}

void EffectTemplate::InifCreateSmoke(PictureLoader *PL) {
	createSmoke.Inif(TypeOfEffect::SMOKE, PL->ReferEffectList(TypeOfEffect::SMOKE));
}