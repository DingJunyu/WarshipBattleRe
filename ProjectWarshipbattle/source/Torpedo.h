#pragma once
#include "AllMovableObjects.h"
/*‹›—‹‚ðŽÀŒ»‚·‚é‚½‚ß‚ÌƒNƒ‰ƒX*/
class Torpedo :
	public AllMovableObjects
{
public:
	Torpedo() : AllMovableObjects(true, false, true, true) {}
	~Torpedo();

private:
	double depth;
};

