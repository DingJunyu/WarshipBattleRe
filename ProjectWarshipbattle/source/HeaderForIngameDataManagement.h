#pragma once
//標準ヘッダー
#include<list>
#include<vector>
#include<stdlib.h>
//DXヘッダー
#include"DxLib.h"
//自作ヘッダー
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
#include"FlagShipAI.h"//敵フラグシープのAI
#include"ArtificialIntelligence.h"//船別のAI

/*編成部分*/
#include"ShipCard.h"

struct EnemyWithAI {
	ShipMain shipHere;
	ArtificialIntelligence AI;
};