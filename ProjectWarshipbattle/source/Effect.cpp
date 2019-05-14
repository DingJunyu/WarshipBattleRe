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

	/*�J�����̍��W�ƕΈڂƍ��̍��W�����킹�āA�����ׂ��Ƃ���̍��W���v�Z����*/
	realX = coordX - x - offsetOnX;
	realZ = coordZ - z - offsetOnZ;

//	if (type == TypeOfEffect::EXPLOSION||type==TypeOfEffect::RIPPLE||
//		type == TypeOfEffect::SMOKE) {
		nextFlame = flame % 64;
		nextFlame /= 4;
//	}

	/*��ʂɓ��鎞�����`����s��*/
	if (realX > 0 && realX < Screen::SCREEN_X&&
		realZ>0 && realZ < Screen::SCREEN_Z) {
		DrawRotaGraph3((int)coordX - x - (int)offsetOnX,
			(int)coordZ - z - (int)offsetOnZ,
			0, 0,
			zoomMutliple, zoomMutliple, radian - MathAndPhysics::PI / 4,
			*(graphicHandle + nextFlame), TRUE, FALSE);
	}

	flame++;

	/*�e�X�g�p*/
	/*unsigned int Cr = GetColor(0, 0, 255);
	DrawPixel(coordX - x, coordZ - z, Cr);*/
}

void Effect::Check() {
	if (spread) {//�g�U���ł����
		zoomMutliple *= zoomRate;
		if (zoomRate > 1)//�g�嗦���t���[�����ƂɌ���
			zoomRate *= 0.99;
		if (zoomRate <= 1)//�ŏ��ɂ��Ă��P�ł�
			zoomRate = 1;
	}

	//�ڕW�p�x�ɂȂ�O�ɉ��
	if (radian != targetRadian && abs(radian - targetRadian) >
		radianChangePerFrame) {
		if (radian < 0)//0��菬��������0�Ɍ������Đ��񂷂�
			radian += radianChangePerFrame;
		if (radian > 0)//0���傫������0�Ɍ������Đ��񂷂�
			radian -= radianChangePerFrame;
	}

	//���̃G�t�F�N�g�̎��Ԃ��؂ꂽ��
	if (GetNowCount() > endTime)
		timeUp = true;
}

void Effect::Move() {
	//�ړ�����O�Ɋp�x�������_���ɕς��
	if (rand() % 2 == 0)//�m����50%
		radian += (double)((rand() % 2) / 180.0f) * MathAndPhysics::PI;
	else
		radian -= (double)((rand() % 2) / 180.0f) * MathAndPhysics::PI;

	//�ړ�����
	coordX += cos(radian)*speed;
	coordZ += sin(radian)*speed;
}