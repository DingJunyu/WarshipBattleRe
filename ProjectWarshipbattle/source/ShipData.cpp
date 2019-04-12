#include "ShipData.h"

ShipData::ShipData()
{
}

ShipData::~ShipData()
{
}

ShipDataCollection::ShipDataCollection() {

}

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

void ShipDataCollection::InifBattleCrusier() {
	const int &BC = BATTLE_CRUISER;
	//リュッツォウd
	BattleCrusierCollection[0].InifSNandST(4000, BC);
}