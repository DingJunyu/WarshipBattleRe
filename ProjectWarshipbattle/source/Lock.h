#pragma once
#include"ShipMain.h"
#include"DefinedData.h"
class Lock
{
public:
	Lock();
	~Lock();

	void CalTheDistance(ShipMain *shipA,ShipMain *shipB);

private:
	double radian;
	double distance;
	int discoverStatus;
};

