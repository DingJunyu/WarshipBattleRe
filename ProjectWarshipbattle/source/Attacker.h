#pragma once
#include"Plane.h"
#include"TorpedoLoader.h"
/*攻撃機を実現する*/
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

