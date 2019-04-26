#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dxlib.h>
#include <signal.h>
#include "DefinedData.h"
#include <cmath>
/*�����v�Z�֐�*/
double Distance3D(Coordinate<double> A, Coordinate<double> B);//�R�c�̋������v�Z����
double Distance2D(Coordinate<double> A, Coordinate<double> B);//�Q�c�̋������v�Z����
double Distance2D(Coordinate2D<double> A, Coordinate2D<double> B);

/*�����蔻��*/
bool crash3DtoPoint(Coordinate<double>A, Coordinate<double> B,
	CrashSize3d<double> ACrash, double radian);//�_�ƒ����`�̂����蔻��[�[���񂵂Ă��ł���

bool crash3Dto3D(Coordinate<double>A, Coordinate<double> B,
	CrashSize3d<double> ACrash, CrashSize3d<double> BCrash,
	double radianA, double radianB);

/*�p�x�v�Z�֐�*/
/*�^�[�Q�b�g�̍��W�A�����̍��W�A�������̌���*/
/*�߂�l�͍��̌����ƖڕW�֌����̊p�x�̍��A�K���s�p��Ԃ�*/
double CalRadianBetweenPoints(Coordinate2D<double> A, Coordinate2D<double> B,
	double radian);

/*���t���[����̈ʒu��\������*/
void NextPoint(Coordinate2D<double> *coord, double radian, double speed, int frames);
/*�����_���Ƀ|�C���g�𐶐�����*/
void RandomPoint(Coordinate2D<double> *coord, double nextX, double nextZ,
	int Range);