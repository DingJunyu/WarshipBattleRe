#include "Ammo.h"

Ammo::~Ammo()
{
}

void Ammo::CheckAlive() {
	//ŠC‚É—Ž‚¿‚½‚çŽg‚¦‚È‚¢‚æ‚¤‚É‚·‚é
	if (ReferCoordY() <= 0) {
		isUsable = false;
	}
}

void Ammo::SetData() {
	GetGraphSize(*ammoHandle, &graphX, &graphZ);
}

void Ammo::Draw(int x,int z){
	DrawRotaGraph3((int)ReferCoordX() - x,
		(int)ReferCoordZ() - z,
		graphX / 2, graphZ / 2,
		0.05, 0.05, ReferRadianOnZ(),
		*ammoHandle, TRUE, FALSE);
}