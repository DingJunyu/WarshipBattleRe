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
double Distance3D(Coordinate<double> A, Coordinate<double> B);//３Ｄの距離を計算する
double Distance2D(Coordinate<double> A, Coordinate<double> B);//２Ｄの距離を計算する

bool crash3DtoPoint(Coordinate<double>A, Coordinate<double> B,
	CrashSize3d<double> ACrash, double radian);//点と長方形のあたり判定ーー旋回してもできる

bool crash3Dto3D(Coordinate<double>A, Coordinate<double> B,
	CrashSize3d<double> ACrash, CrashSize3d<double> BCrash,
	double radianA, double radianB);