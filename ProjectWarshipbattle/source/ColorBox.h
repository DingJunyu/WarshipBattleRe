#pragma once
#include"DxLib.h"

enum COLOR_MARK {
	CR_AMOUNT = 20,
	GREEN = 0,
	RED,
	BLUE,
	BLACK,
	WHITE,
	MIDNIGHT,//�u���b�N�ɋ߂�
	GRENADIER,//�Ԃɋ߂�
	LOCHMARA,//�ɋ߂�
	MY_SIN,//���F�ɋ߂�
	TENNE,//�I�����W�ɋ߂�
	MEDIUM_SEA_GREEN,
	TERRA_COTTA,//�󂢐� #EC7603
	DROVER,//�󂢉��F #F9E79F
	GORSE,//���F #FFEB3B  
	RED_ORANGE,//#F44336
	IRON_STONE//���F���ۂ�? #795548
};

class ColorBox
{
public:
	ColorBox();
	~ColorBox();

	unsigned int ReferColor(int num) { return CrBox[num]; }

private:
	unsigned int CrBox[CR_AMOUNT];
	void SetColorBox();
};
