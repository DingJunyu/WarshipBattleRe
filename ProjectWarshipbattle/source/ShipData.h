#pragma once
#include"DefinedData.h"
//���ׂĂ̑D�̃f�[�^�͂����ŕۑ����Ēu���܂�
/*�t�@�C������D�̃X�e�[�^�X��ǂݍ��ނȂ�*/
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
	//�D�ԍ�
	int serialNum;
	//�D�^
	int shipType;

	//�����蔻��f�[�^
	int crashPartsCount;
	int crashPartX;
	int crashPartZ;
};

