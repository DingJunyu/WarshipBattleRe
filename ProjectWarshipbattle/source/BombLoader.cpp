#include "BombLoader.h"

BombLoader::~BombLoader()
{
}

Bomb BombLoader::DropTheBomb() {
		UseAmmo();//�c�e������
		return bombOnThisPlane;//���e�̏�Ԃ�Ԃ�
}