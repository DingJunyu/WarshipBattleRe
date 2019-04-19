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
double Distance3D(Coordinate<double> A, Coordinate<double> B);//�R�c�̋������v�Z����
double Distance2D(Coordinate<double> A, Coordinate<double> B);//�Q�c�̋������v�Z����

bool crash3DtoPoint(Coordinate<double>A, Coordinate<double> B,
	CrashSize3d<double> ACrash, double radian);//�_�ƒ����`�̂����蔻��[�[���񂵂Ă��ł���

bool crash3Dto3D(Coordinate<double>A, Coordinate<double> B,
	CrashSize3d<double> ACrash, CrashSize3d<double> BCrash,
	double radianA, double radianB);