#pragma once
#include "AllMovableObjects.h"
/*�������������邽�߂̃N���X*/
class Torpedo :
	public AllMovableObjects
{
public:
	Torpedo() : AllMovableObjects(true, false, true, true) {}
	~Torpedo();

private:
	double depth;
};

