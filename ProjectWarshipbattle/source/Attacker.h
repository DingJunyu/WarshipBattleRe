#pragma once
#include"Plane.h"
#include"TorpedoLoader.h"
/*�U���@����������*/
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

