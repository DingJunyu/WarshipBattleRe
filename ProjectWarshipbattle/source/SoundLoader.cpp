#include "SoundLoader.h"

SoundLoader::SoundLoader()
{
}

SoundLoader::~SoundLoader()
{
}

void SoundLoader::Inif() {
	LoadSound();
}

void SoundLoader::FreeAll() {
	DeleteSound();
}

void SoundLoader::LoadSound() {
	shipSoundEngine = LoadSoundMem(
	"SoundData/Ships/Engine/Ship_Sound_Engine_Moving.mp3");

	shipSoundMoving = LoadSoundMem(
	"SoundData/Ships/Moving/Ship_Sound_Moving.mp3");

	shipSoundSlow = LoadSoundMem(
	"SoundData/Ships/Moving/Ship_Sound_Slow.mp3");

	shipSoundTurning = LoadSoundMem(
	"SoundData/Ships/Turning/Ship_Sound_Turning.mp3");

	shipHordLargeHigh = LoadSoundMem(
	"SoundData/Ships/Hord/Ship_Hord_High.mp3");

	shipHordLargeLow = LoadSoundMem(
	"SoundData/Ships/Hord/Ship_Hord_Low.mp3");
}

void SoundLoader::DeleteSound() {
	DeleteSoundMem(shipSoundEngine);
	DeleteSoundMem(shipSoundMoving);
	DeleteSoundMem(shipSoundTurning);
	DeleteSoundMem(shipHordLargeHigh);
	DeleteSoundMem(shipHordLargeLow);
}