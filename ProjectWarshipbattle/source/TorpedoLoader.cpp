#include "TorpedoLoader.h"

TorpedoLoader::~TorpedoLoader()
{
}

Torpedo TorpedoLoader::DropTheTorpedo() {
		UseAmmo();//�c�e������
		return torpedoOnThisPlane;//�����̏�Ԃ�Ԃ�
}