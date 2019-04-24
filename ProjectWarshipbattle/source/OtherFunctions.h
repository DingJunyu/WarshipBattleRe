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
/*距離計算関数*/
double Distance3D(Coordinate<double> A, Coordinate<double> B);//３Ｄの距離を計算する
double Distance2D(Coordinate<double> A, Coordinate<double> B);//２Ｄの距離を計算する

/*あたり判定*/
bool crash3DtoPoint(Coordinate<double>A, Coordinate<double> B,
	CrashSize3d<double> ACrash, double radian);//点と長方形のあたり判定ーー旋回してもできる

bool crash3Dto3D(Coordinate<double>A, Coordinate<double> B,
	CrashSize3d<double> ACrash, CrashSize3d<double> BCrash,
	double radianA, double radianB);

/*角度計算関数*/
/*ターゲットの座標、自分の座標、今自分の向き*/
/*戻り値は今の向きと目標へ向きの角度の差、必ず鋭角を返す*/
double CalRadianBetweenPoints(Coordinate2D<double> A, Coordinate2D<double> B,
	double radian);