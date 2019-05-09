#pragma once
#include"DefinedData.h"
//すべての船のデータはここで保存して置きます
/*ファイルから船のステータスを読み込むなど*/
class ShipData
{
public:
	ShipData();
	~ShipData();

	void InifSNandST(int SN, int ST) { 
		serialNum = SN;
		shipType = ST;
	}
	void InifCrashParts();

private:
	//船番号
	int serialNum;
	//船型
	int shipType;

	//当たり判定データ
	int crashPartsCount;
	int crashPartX;
	int crashPartZ;
};

