#include "ShipData.h"
/*�D�̃f�[�^��ǂݍ��ރN���X�ł����A
�����͂܂��ł�*/

ShipData::ShipData()
{
}

ShipData::~ShipData()
{
}

ShipDataCollection::ShipDataCollection() {

}

/*���������m�ۂ���*/
void ShipDataCollection::AllInif() {
	MemorySecure();

	InifBattleCrusier();
}

void ShipDataCollection::MemorySecure() {
	BattleCrusierCollection 
		= new ShipData[COUNT_OF_EVERYTYPE::BATTLE_CRUSIER_TYPE_COUNT];
}

void ShipDataCollection::FreeMemory() {
	delete[] BattleCrusierCollection;
}

/*�ԍ�������������...?*/
void ShipDataCollection::InifBattleCrusier() {
	const int &BC = BATTLE_CRUISER;
	//�����b�c�H�Ed
	BattleCrusierCollection[0].InifSNandST(4000, BC);
}