#include "Effect.h"

Effect::Effect() {

}

Effect::~Effect()
{
}

void Effect::Draw(int x,int z) {
	double rotateX = 0;
	double rotateZ = 0;
	double offsetOnX;
	double offsetOnZ;

	offsetOnX = zoomMutliple * zoomMutliple * graphX * 0.25 ;//�w���W�̕Έڂ��v�Z
	offsetOnZ = zoomMutliple * zoomMutliple * graphZ * 0.25 ;//�y���W�̕Έڂ��v�Z
	
	double realX, realZ;

	realX = coordX - x - offsetOnX;
	realZ = coordZ - z - offsetOnZ;

	/*��ʂɓ��鎞�����`����s��*/
	if (realX > 0 && realX < Screen::SCREEN_X&&
		realZ>0 && realZ < Screen::SCREEN_Z) {
		DrawRotaGraph3((int)coordX - x - (int)offsetOnX,
			(int)coordZ - z - (int)offsetOnZ,
			0, 0,
			zoomMutliple, zoomMutliple, radian - MathAndPhysics::PI / 4,
			*graphicHandle, TRUE, FALSE);
	}

	/*�e�X�g�p*/
	/*unsigned int Cr = GetColor(0, 0, 255);
	DrawPixel(coordX - x, coordZ - z, Cr);*/
}

void Effect::Check() {
	if (spread) {//�g�U���ł����
		zoomMutliple *= zoomRate;
		if (zoomRate > 1)//�g�嗦���t���[�����ƂɌ���
			zoomRate *= 0.99;
		if (zoomRate <= 1)
			zoomRate = 1;
	}

	//�ڕW�p�x�ɂȂ�O�ɉ��
	if (radian != targetRadian && abs(radian - targetRadian) >
		radianChangePerFrame) {
		if (radian < 0)
			radian += radianChangePerFrame;
		if (radian > 0)
			radian -= radianChangePerFrame;
	}

	//���̃G�t�F�N�g�̎��Ԃ��؂ꂽ��
	if (GetNowCount() > endTime)
		timeUp = true;
}

void Effect::Move() {
	//�ړ�����O�Ɋp�x�������_���ɕς��
	if (rand()%2==0)
		radian += (double)((rand() % 2) / 180.0f) * MathAndPhysics::PI;
	else
		radian -= (double)((rand() % 2) / 180.0f) * MathAndPhysics::PI;

	coordX += cos(radian)*speed;
	coordZ += sin(radian)*speed;
}