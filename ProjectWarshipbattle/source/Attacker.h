#pragma once
#include"Plane.h"
#include"TorpedoLoader.h"
class Attacker :
	public Plane
{
public:
	Attacker();
	~Attacker();

	void Inif();

	TorpedoLoader MountPoint;
private:
	
};

