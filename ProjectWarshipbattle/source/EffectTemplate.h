#pragma once
#include"EffectPoint.h"
#include"PictureLoader.h"
class EffectTemplate
{
public:
	EffectTemplate();
	~EffectTemplate();

	void InifEffectTemplate(PictureLoader *PL);

	EffectPoint CopyFromCreateBubble() { return createBubble; }

	EffectPoint CopyFromCreateSmoke() { return createSmoke; }

private:
	void InifCreateBubble(PictureLoader *PL);
	void InifCreateSmoke(PictureLoader *PL);

	/*エフェクトタイプはTypeOfEffectに参考*/
	EffectPoint createBubble;
	EffectPoint createSmoke;
};

