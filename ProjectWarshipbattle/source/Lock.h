#pragma once
#include"ShipMain.h"
#include"DefinedData.h"
class Lock
{
public:
	Lock();
	~Lock();

private:
	ShipMain *myShip;
	ShipMain *target;
	double radian;
	double distance;
	int discoverStatus;
};

