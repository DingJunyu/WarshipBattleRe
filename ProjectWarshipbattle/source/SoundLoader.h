#pragma once
#include"DxLib.h"

class SoundLoader
{
public:
	SoundLoader();
	~SoundLoader();

	/*‰Šú‰»ŠÖ”*/
	void Inif();
	/*‰ğ•úŠÖ”*/
	void FreeAll();

	int * referShipSoundEngine() { return &shipSoundEngine; }

	int * referShipSoundMoving() { return &shipSoundMoving; }
	int * referShipSoundSlow() { return &shipSoundSlow; }
	int * referShipSoundTuring() { return &shipSoundTurning; }

	int * referShipHordLargeHigh() { return &shipHordLargeHigh; }
	int * referShipHordLargeLow() { return &shipHordLargeLow; }

private:

	void LoadSound();
	void DeleteSound();

	int shipSoundEngine;

	int shipSoundMoving;
	int shipSoundSlow;
	int shipSoundTurning;

	int shipHordLargeHigh;
	int shipHordLargeLow;
};