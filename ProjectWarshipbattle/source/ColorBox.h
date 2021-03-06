#pragma once
#include"DxLib.h"

enum COLOR_MARK {
	CR_AMOUNT = 20,
	GREEN = 0,
	RED,
	BLUE,
	BLACK,
	WHITE,
	MIDNIGHT,//ブラックに近い
	BLUE_CHARCOAL,//#1C2833 ブラックに近い2
	GRENADIER,//赤に近い
	LOCHMARA,//青に近い
	MY_SIN,//黄色に近い
	TENNE,//オレンジに近い
	MEDIUM_SEA_GREEN,
	TERRA_COTTA,//浅い赤 #EC7603
	DROVER,//浅い黄色 #F9E79F
	GORSE,//黄色 #FFEB3B  
	RED_ORANGE,//#F44336
	IRON_STONE,//茶色っぽい? #795548
	WATER_COURSE,//深い緑 #0E6655 
	ASTRONAUT_BLUE//深いブルー #154360  
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
