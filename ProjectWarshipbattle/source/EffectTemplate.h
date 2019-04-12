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

	/*�G�t�F�N�g�^�C�v��TypeOfEffect�ɎQ�l*/
	EffectPoint createBubble;
	EffectPoint createSmoke;
};

