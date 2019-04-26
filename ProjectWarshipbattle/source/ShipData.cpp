#include "ShipData.h"
/*船のデータを読み込むクラスですが、
実現はまだです*/

ShipData::ShipData()
{
}

ShipData::~ShipData()
{
}

ShipDataCollection::ShipDataCollection() {

}

/*メモリを確保する*/
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

/*番号を初期化する...?*/
void ShipDataCollection::InifBattleCrusier() {
	const int &BC = BATTLE_CRUISER;
	//リュッツォウd
	BattleCrusierCollection[0].InifSNandST(4000, BC);
}