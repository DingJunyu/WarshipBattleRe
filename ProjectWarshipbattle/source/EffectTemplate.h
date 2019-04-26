#pragma once
#include"EffectPoint.h"
#include"PictureLoader.h"
/*�G�t�F�N�g��EffectPoint���甭�����邪�A
EffectTemplate��EffectPoint�̃X�e�[�^�X��ۑ����āA
�V����EffectPoint�𐶐����鎞�͂�������R�s�[����*/
class EffectTemplate
{
public:
	EffectTemplate();
	~EffectTemplate();

	void InifEffectTemplate(PictureLoader *PL);

	/*�A�����_���R�s�[����*/
	EffectPoint CopyFromCreateBubble() { return createBubble; }

	/*�_�����_���R�s�[����*/
	EffectPoint CopyFromCreateSmoke() { return createSmoke; }

private:
	void InifCreateBubble(PictureLoader *PL);
	void InifCreateSmoke(PictureLoader *PL);

	/*�G�t�F�N�g�^�C�v��TypeOfEffect�ɎQ�l*/
	EffectPoint createBubble;
	EffectPoint createSmoke;
};

