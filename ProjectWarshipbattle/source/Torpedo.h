#pragma once
#include "AllMovableObjects.h"
class Torpedo :
	public AllMovableObjects
{
public:
	Torpedo() : AllMovableObjects(true, false, true, true) {}
	~Torpedo();

private:
	double depth;
};

