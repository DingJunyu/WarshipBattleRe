#pragma once
#include "AllMovableObjects.h"
/*魚雷を実現するためのクラス*/
class Torpedo :
	public AllMovableObjects
{
public:
	Torpedo() : AllMovableObjects(true, false, true, true) {}
	~Torpedo();

private:
	double depth;
};

