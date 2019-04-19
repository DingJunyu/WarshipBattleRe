#pragma once
#include"Plane.h"
#include"TorpedoLoader.h"
/*UŒ‚‹@‚ğÀŒ»‚·‚é*/
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

