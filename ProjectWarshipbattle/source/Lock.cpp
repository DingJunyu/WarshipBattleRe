#include "Lock.h"

Lock::Lock()
{
}

Lock::~Lock()
{
}

void Lock::CalTheDistance(ShipMain *shipA, ShipMain *shipB) {
	distance = Distance2D(shipA->ReferCoord(), shipB->ReferCoord());
}