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
double Distance3D(Coordinate<double> A, Coordinate<double> B);//‚R‚c‚Ì‹——£‚ğŒvZ‚·‚é
double Distance2D(Coordinate<double> A, Coordinate<double> B);//‚Q‚c‚Ì‹——£‚ğŒvZ‚·‚é

bool crash3DtoPoint(Coordinate<double>A, Coordinate<double> B,
	CrashSize3d<double> ACrash, double radian);//“_‚Æ’·•ûŒ`‚Ì‚ ‚½‚è”»’è[[ù‰ñ‚µ‚Ä‚à‚Å‚«‚é

bool crash3Dto3D(Coordinate<double>A, Coordinate<double> B,
	CrashSize3d<double> ACrash, CrashSize3d<double> BCrash,
	double radianA, double radianB);