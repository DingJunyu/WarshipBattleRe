#pragma once
#include"EffectPoint.h"
#include"PictureLoader.h"
/*エフェクトはEffectPointから発生するが、
EffectTemplateにEffectPointのステータスを保存して、
新しいEffectPointを生成する時はここからコピーする*/
class EffectTemplate
{
public:
	EffectTemplate();
	~EffectTemplate();

	void InifEffectTemplate(PictureLoader *PL);

	/*泡生成点をコピーする*/
	EffectPoint CopyFromCreateBubble() { return createBubble; }

	/*雲生成点をコピーする*/
	EffectPoint CopyFromCreateSmoke() { return createSmoke; }

private:
	void InifCreateBubble(PictureLoader *PL);
	void InifCreateSmoke(PictureLoader *PL);

	/*エフェクトタイプはTypeOfEffectに参考*/
	EffectPoint createBubble;
	EffectPoint createSmoke;
};

