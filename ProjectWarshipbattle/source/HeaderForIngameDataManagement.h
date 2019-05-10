#pragma once
//�W���w�b�_�[
#include<list>
#include<vector>
#include<stdlib.h>
//DX�w�b�_�[
#include"DxLib.h"
//����w�b�_�[
#include"DefinedData.h"
#include"Camera.h"
#include"UserInterface.h"
#include"ShipMain.h"
#include"Ammo.h"
#include"Bomb.h"
#include"Torpedo.h"
#include"PictureLoader.h"
#include"ShipData.h"
#include"FrameControl.h"
#include"Controller.h"
#include"EffectTemplate.h"
#include"Effect.h"
#include"SoundLoader.h"
#include"OtherFunctions.h"
#include"ClickableUserInterface.h"

//AI
#include"FlagShipAI.h"//�G�t���O�V�[�v��AI
#include"ArtificialIntelligence.h"//�D�ʂ�AI

/*�Ґ�����*/
#include"ShipCard.h"

struct EnemyWithAI {
	ShipMain shipHere;
	ArtificialIntelligence AI;
};