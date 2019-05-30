#include "IngameDataManagement.h"
/*�C�e���[�^���g���O�ɕK���e��̏󋵂��m�F���܂��I�I�I�I�I*/
using namespace DxLib;
IngameDataManagement::~IngameDataManagement()
{
}

/*���C�����[�g*/
void IngameDataManagement::Update() {
	AIUpdate();//AI�Ńf�[�^���X�V������
	LockAndRefresh();//���b�N��Ԃ��X�V������

	/*�J������Ԃ�ݒu����*/
	if (alliesFleet[0].fireDataFigureUp.ReferLockOn() && showLock == true)
		MainCamera.GetXZ(ReferTargetX(alliesFleet[0].fireDataFigureUp.ReferTarget()),
			ReferTargetZ(alliesFleet[0].fireDataFigureUp.ReferTarget()));
	else
		MainCamera.GetXZ(ReferPlayerX(), ReferPlayerZ());//�J�������W���X�V
		
	Control();//�R�}���h���󂯎���āA�D�̏�Ԃ�ύX����
	GetNewEffect();//�G�t�F�N�g�𐶐�����
	MoveAll();//�ړ��A��ԍX�V

	SinkingListUpdate();

	if (!GameEnd)
		CheckTeamStatus();//�e�͑��̏�Ԃ��m�F���A������r�ł��ꂽ��Q�[���I��

	DeleteUseless();//����Ȃ����̂�����
	RemoveSinkedShip();

	if (GameEnd)
		EndTheGame();

	CheckAndPlaySound();
	if (ShouldIDraw())
		DrawAll();//�S���X�V�������ʂ�`��
	shouldIDraw = FC.Wait();
}

/*�Q�[���R���g���[��*/
/*�ۑ��������ς�����̂��߁A�㒼��*/
bool IngameDataManagement::TeamDestroyed() {
	if (alliesFleet.empty() || enemyFleet.empty())
		return false;
	return false;
}

/****************************************************/
/*                      AI�Ǘ�                      */
/****************************************************/
void IngameDataManagement::AIUpdate() {
	LetFlagShipMove();//�G�̃t���O�V�[�v�𓮂���
	LetEveryOneMove();//���ׂĂ̑D�̏�Ԃ��X�V����
	LetEveryOneLockOn();//�^�[�Q�b�g��I������
	LetEveryOneShoot();//�ˌ�����
}

void IngameDataManagement::LetFlagShipMove() {
	if (!enemyFlagShip->ReferAlive())//�����t���O�V�[�v�͂܂������Ă����
		return;
	flagShipAI.LetUsGo(enemyFlagShip, &alliesFleet[0]);//�v���[���[�����삵�Ă���D���^�[�Q�b�g�ɐݒ肵�ړ�����
	enemyFlagShip->SetChangingDirect(flagShipAI.ReferRadianNeededNow());//�p�x���X�V����
	enemyFlagShip->SetEngineOutPutRate(flagShipAI.ReferSpeedInNeed());//�o�͂��X�V����
}

void IngameDataManagement::LetEveryOneMove() {
	ControlThisListMove(&alliesFleet, &AI);//�F�R�͑����ړ�����
	ControlThisListMove(&enemyFleet, &AI);//�G�R�͑����ړ�����
}

void IngameDataManagement::ControlThisListMove(std::vector<ShipMain> *shipList,
	ArtificialIntelligence *AI) {	
	bool first = true;
	if (!shipList->empty() && shipList->begin()!=shipList->end()) {
		for (auto ship = shipList->begin();
			ship != shipList->end();
			ship++) {
			if (first) {//��Ԗڂ��X�L�b�v����
				first = false;
				continue;
			}
			if (!ship->ReferAlive())//���񂾂�X�L�b�v����
				continue;
			if (&*ship == enemyFlagShip)//�t���O�V�[�v��������X�L�b�v����
				continue;
			ship--;
			auto prevShip = ship;//�O�̑D��ݒ肷��
			ship++;
			while (1) {
				if (prevShip->ReferAlive())//�O�̑D�͂܂������Ă���΂��̂܂܎g��
					break;
				if (prevShip == shipList->begin()) {//������ԑO�̑D�����񂾂�
					enemyFlagShip = &*ship;//���̑D���t���b�O�V�[�v�ɐݒ肷��
					return;//�X�V����߂�
				}
				if (prevShip != shipList->begin())//�܂��O�Ɉړ��ł����
					prevShip--;
			}
			AI->Move(*ship, *prevShip);//�ړ�����
			ship->SetChangingDirect(AI->ReferRadianNeededNow());//�ύX�p�x��ݒu����
			ship->SetEngineOutPutRate(AI->ReferOutPutRateNeededNow());//���x��ݒu����
		}
	}
}

void IngameDataManagement::LetEveryOneLockOn() {
	ControlThisListLock(&alliesFleet, enemyFleet);
	ControlThisListLock(&enemyFleet, alliesFleet);
}

void IngameDataManagement::ControlThisListLock(std::vector<ShipMain> *shipList,
	std::vector<ShipMain> enemyList) {
	for (auto ship = shipList->begin();
		ship != shipList->end();
		ship++) {
		if (ship->ReferAlive() && !ship->ReferControled()) {
			AI.InBattle(&*ship, enemyList, -1);//���b�N�^�[�Q�b�g��ݒu����
			ship->fireDataFigureUp.LockOn_Switch();//���b�N��Ԃ��X�V����
		}
	}
}

void IngameDataManagement::LetEveryOneShoot() {
	ControlThisListShoot(&alliesFleet);
	ControlThisListShoot(&enemyFleet);
}

void IngameDataManagement::ControlThisListShoot(std::vector<ShipMain> *shipList) {
	for (auto ship = shipList->begin();
		ship != shipList->end();
		ship++) {
		if (ship->ReferAlive() && //�����Ă���
			(!ship->ReferControled()||
			autoFire) &&//�v���[���[�̑D����Ȃ��������̓v���[���[�͎����ˌ��g���Ă��Ȃ�
			ship->ReferCanIShoot()) {//�ˌ��͈͓��ɂ���
			if (ship->ReferSerialNumber() != 0)
				TestShoot(&*ship, false);//�ˌ�
			else
				TestShoot(&*ship, true);
		}
	}
}

/****************************************************/
/*                     �`��֘A                     */
/****************************************************/
void IngameDataManagement::DrawAll() {
	SetDrawScreen(DX_SCREEN_BACK);//����ʂɕ`�悷��
	ClearDrawScreen();//��ʂ��N���A����

	auto ship = alliesFleet.begin();//�C�e���[�^�ő��삵�Ă���D�̃X�e�[�^�X�����

	/*�C��`��*/
	DrawSea_New_Extend();
	DrawMesh_Sea_Extend();

	//�D�̉��̃G�t�F�N�g��`�悷��
	DrawEffectUnderShips();
	/*�D�̂����ɂ���UI��`�悷��*/
	UI.DrawUIUnderShip(ship->fireDataFigureUp.ReferLockOn(),
		enemyFleet[ship->fireDataFigureUp.ReferTarget()].ReferCoord2D_d(),
		alliesFleet[0].ReferCoord2D_d(),
		MainCamera, ship->ReferRadianOnZ() + ship->ReferChangingRadian());
//	DrawTorpedo();
	DrawShips();//�D��`�悷��
	DrawEffectBeyondShips();//�D�̏�̃G�t�F�N�g��`�悷��
//	DrawPlanes();
	DrawAmmo();//�e��`�悷��
//	DrawBomb();

	DrawPointOfImpact();//�C�e�����\���n�_�`��

	/*UI�֘A*/
	UI.DrawUI();//���ʂ�UI��`�悷��
	UI.DrawUINeedInput(&*ship, autoFire,//�D�A�����ˌ����
		(alliesFleet[0].fireDataFigureUp.ReferLockOn() && showLock));//�J�����ڕW
	//�D�̃X�e�[�^�X�Ɋ֌W������UI��`�悷��
	DrawShipsOnMiniMap();//�~�j�}�b�v��`��
	/*�N���b�N�\��UI*/
	CUI.Draw();//�{�^����`��
	CUI.DrawNeedInput(ship->fireDataFigureUp.ReferLockOn(), 
		ship->fireDataFigureUp.ReferTarget());//�������K�v�ȕ�����`��
	DrawHPBar();
	

	/*�e�X�g�C���t�H���[�V����*/
	if (TEST_SHOW_ON)
		TEST_DRAW();

	if (GameEnd)
		UI.DrawWinOrLose(win);

	DrawErrorList();
	
	DxLib::ScreenFlip();
}

/*�{�̂Ɖe�𕪂��ĕ`�悷��*/
void IngameDataManagement::DrawShips() {
	//�F�R��`�悷�� 
	//�R���g���[�����Ă���D���F�R�͑��̈�Ԃɂ���
	if (!alliesFleet.empty())//���X�g�󋵂��m�F
		for (auto mark = alliesFleet.begin();
			mark != alliesFleet.end(); mark++) {
		if (mark->ReferAlive()) {//�^�[�Q�b�g�̏�Ԃ��m�F
//			if (mark == alliesFleet.begin())
//				mark->Draw(MainCamera);//������������ʒ��S�ɉf��
//			else
				mark->DrawSub(MainCamera);//����ȊO�̂͑��΍��W�𗘗p���ĕ`��
		}
	}
	if(!enemyFleet.empty())//���X�g�󋵂��m�F
		for (auto ship = enemyFleet.begin();
			ship != enemyFleet.end(); ship++) {
		if (ship->ReferAlive()) {//�^�[�Q�b�g�̏�Ԃ��m�F
			ship->DrawSub(MainCamera);//�G�R�͑S�����΍��W�𗘗p���ĕ`��
		}
	}

	if (!sinkingShip.empty()) {
		for (auto ship = sinkingShip.begin();
			ship != sinkingShip.end(); ship++) {
			ship->DrawSub(MainCamera);
		}
	}
}

void IngameDataManagement::DrawShipsShadow() {
	//�F�R��`�悷�� 
	//�R���g���[�����Ă���D���F�R�͑��̈�Ԃɂ���
	if (!alliesFleet.empty())//���X�g�󋵂��m�F
		for (auto mark = alliesFleet.begin();
			mark != alliesFleet.end(); mark++) {
		if (mark->ReferAlive()) {//�^�[�Q�b�g�̏�Ԃ��m�F
//			if (mark == alliesFleet.begin())
//				mark->DrawShadow(MainCamera);//������������ʒ��S�ɉf��
//			else
				mark->DrawSubShadow(MainCamera);//����ȊO�̂͑��΍��W�𗘗p���ĕ`��
		}
	}
	if (!enemyFleet.empty())//���X�g�󋵂��m�F
		for (auto ship = enemyFleet.begin();
			ship != enemyFleet.end(); ship++) {
		if (ship->ReferAlive()) {//�^�[�Q�b�g�̏�Ԃ��m�F
			ship->DrawSubShadow(MainCamera);//�G�R�͑S�����΍��W�𗘗p���ĕ`��
		}
	}
	if (!sinkingShip.empty()) {
		for (auto ship = sinkingShip.begin();
			ship != sinkingShip.end(); ship++) {
			ship->DrawSubShadow(MainCamera);
		}
	}
}

void IngameDataManagement::DrawShipsOnMiniMap() {
	//�F�R�͑�
	if(!alliesFleet.empty())//���X�g�󋵂��m�F
	for (auto ship = alliesFleet.begin();
		ship != alliesFleet.end(); ship++) {
		if (ship->ReferAlive()) {//�^�[�Q�b�g�̏�Ԃ��m�F
			UI.DrawShipOnTheMap(ship->ReferCoordX(),
				ship->ReferCoordZ(), false);//���}�[�N
		}
	}
	if (!enemyFleet.empty())//���X�g�󋵂��m�F
		for (auto ship = enemyFleet.begin();
			ship != enemyFleet.end(); ship++) {
		if (ship->ReferAlive()) {//�^�[�Q�b�g�̏�Ԃ��m�F
			UI.DrawShipOnTheMap(ship->ReferCoordX(),
				ship->ReferCoordZ(), true);//�Ԃ��}�[�N
		}
	}
}

void IngameDataManagement::SinkingListUpdate() {
	if (!sinkingShip.empty()) {
		for (auto ship = sinkingShip.begin();
			ship != sinkingShip.end(); ship++) {
			SinkingShipUpdate(&*ship);
		}
	}
}

void IngameDataManagement::SinkingShipUpdate(ShipMain *ship) {
	int num = rand() % 2;

	ship->countSinkingFlame();

	if (num) {
		Coordinate2D<double> temp = ship->ReferCoord2D_d();

		temp.x -= ship->ReferShipCrashSize().x / 16;
		temp.z -= ship->ReferShipCrashSize().z / 16;

		double randX, randZ;

		randX = (double)(rand() % (int)ship->ReferShipCrashSize().x/2);
		if (rand() % 2 == 0)
			randX = -randX;
		randZ = (double)(rand() % (int)ship->ReferShipCrashSize().z/2);
		if (rand() % 2 == 0)
			randZ = -randZ;

		temp.x += cos(ship->ReferRadianOnZ()) * randX -
			sin(ship->ReferRadianOnZ()) * randZ;
		temp.z += cos(ship->ReferRadianOnZ()) * randZ +
			sin(ship->ReferRadianOnZ()) * randX;

		NewExplosion(temp);
	}
}

void IngameDataManagement::DrawSea() {
	double mapX = PL.ReferMapX();
	double mapZ = PL.ReferMapZ();

	double MCPOX = MainCamera.ReferPrintOutX(mapX);
	double MCPOZ = MainCamera.ReferPrintOutZ(mapZ);

	int graphNumOnX = 2;
	int graphNumOnZ = 2;

	graphNumOnX = (int)(MainCamera.ReferCameraX() / mapX);
	graphNumOnZ = (int)(MainCamera.ReferCameraZ() / mapZ);

	//�ꖇ�̉摜���g���ĘA���`��
	{
		DrawRectGraph(//����
			(int)(-MCPOX - mapX),
			(int)(-MCPOZ - mapZ),
			(int)0,
			(int)0,
			(int)mapX, (int)mapZ,
			*PL.ReferMapHandle(), FALSE, FALSE);

		DrawRectGraph(//��
			(int)(-MCPOX),
			(int)(-MCPOZ - mapZ),
			(int)0,
			(int)0,
			(int)mapX, (int)mapZ,
			*PL.ReferMapHandle(), FALSE, FALSE);

		DrawRectGraph(//�E��
			(int)(-MCPOX + mapX),
			(int)(-MCPOZ - mapZ),
			(int)0,
			(int)0,
			(int)mapX, (int)mapZ,
			*PL.ReferMapHandle(), FALSE, FALSE);

		DrawRectGraph(//��
			(int)(-MCPOX - mapX),
			(int)(-MCPOZ),
			(int)0,
			(int)0,
			(int)mapX, (int)mapZ,
			*PL.ReferMapHandle(), FALSE, FALSE);

		DrawRectGraph(//��
			(int)(-MCPOX),
			(int)(-MCPOZ),
			(int)0,
			(int)0,
			(int)mapX, (int)mapZ,
			*PL.ReferMapHandle(), FALSE, FALSE);

		DrawRectGraph(//�E
			(int)(-MCPOX + mapX),
			(int)(-MCPOZ),
			(int)0,
			(int)0,
			(int)mapX, (int)mapZ,
			*PL.ReferMapHandle(), FALSE, FALSE);

		DrawRectGraph(//����
			(int)(-MCPOX - mapX),
			(int)(-MCPOZ + mapZ),
			(int)0,
			(int)0,
			(int)mapX, (int)mapZ,
			*PL.ReferMapHandle(), FALSE, FALSE);

		DrawRectGraph(//��
			(int)(-MCPOX),
			(int)(-MCPOZ + mapZ),
			(int)0,
			(int)0,
			(int)mapX, (int)mapZ,
			*PL.ReferMapHandle(), FALSE, FALSE);

		DrawRectGraph(//�E��
			(int)(-MCPOX + mapX),
			(int)(-MCPOZ + mapZ),
			(int)0,
			(int)0,
			(int)mapX, (int)mapZ,
			*PL.ReferMapHandle(), FALSE, FALSE); 
	}
}

void IngameDataManagement::DrawSea_New() {
	double mapX = PL.ReferMapX();
	double mapZ = PL.ReferMapZ();
	double MCPOX, MCPOZ;

	if (MainCamera.ReferCameraX() > 0)
		MCPOX = Screen::SCREEN_X / 2 - abs(MainCamera.ReferPrintOutX(mapX / 2));
	else
		MCPOX = Screen::SCREEN_X / 2 + abs(MainCamera.ReferPrintOutX(mapX / 2));

	if (MainCamera.ReferCameraZ() > 0)
		MCPOZ = Screen::SCREEN_Z / 2 - abs(MainCamera.ReferPrintOutZ(mapZ / 2));
	else
		MCPOZ = Screen::SCREEN_Z / 2 + abs(MainCamera.ReferPrintOutZ(mapZ / 2));

	DrawGraph((int)MCPOX, (int)MCPOZ, *PL.ReferMapHandle(), TRUE);
	DrawGraph((int)MCPOX - (int)mapX, (int)MCPOZ, *PL.ReferMapHandle(), TRUE);
	DrawGraph((int)MCPOX, (int)MCPOZ - (int)mapZ, *PL.ReferMapHandle(), TRUE);
	DrawGraph((int)MCPOX - (int)mapX, (int)MCPOZ - (int)mapZ, *PL.ReferMapHandle(), TRUE);
}

void IngameDataManagement::DrawSea_New_Extend() {
	double mapX = PL.ReferMapX();
	double mapZ = PL.ReferMapZ();
	double MCPOX, MCPOZ;
	double multiple = MainCamera.ReferZoomRatio();

	if (MainCamera.ReferCameraX() > 0)
		MCPOX = Screen::SCREEN_X / 2 -
		abs(MainCamera.ReferPrintOutX(mapX * multiple / 2));
	else
		MCPOX =
		abs(MainCamera.ReferPrintOutX(mapX * multiple / 2));

	if (MainCamera.ReferCameraZ() > 0)
		MCPOZ = Screen::SCREEN_Z / 2 -
		abs(MainCamera.ReferPrintOutZ(mapZ * multiple / 2));
	else
		MCPOZ =
		abs(MainCamera.ReferPrintOutZ(mapZ * multiple / 2));

	DrawExtendGraph((int)MCPOX, (int)MCPOZ,
		(int)(MCPOX + mapX * multiple), 
		(int)(MCPOZ + mapZ * multiple),
		*PL.ReferMapHandle(), TRUE);
	DrawExtendGraph((int)(MCPOX - mapX * multiple),
		(int)MCPOZ,
		(int)(MCPOX),
		(int)(MCPOZ + mapZ  * multiple),
		*PL.ReferMapHandle(), TRUE);
	DrawExtendGraph((int)MCPOX, 
		(int)(MCPOZ - mapZ * multiple),
		(int)(MCPOX + mapX * multiple),
		(int)(MCPOZ),
		*PL.ReferMapHandle(), TRUE);
	DrawExtendGraph((int)(MCPOX - mapX * multiple),
		(int)(MCPOZ - mapZ * multiple),
		(int)(MCPOX),
		(int)(MCPOZ),
		*PL.ReferMapHandle(), TRUE);
}

void IngameDataManagement::DrawMesh_Sea_Extend() {
	double mapX = PL.ReferMapX();
	double mapZ = PL.ReferMapZ();
	double multiple = MainCamera.ReferZoomRatio();

	double MCPOX;
	double MCPOZ;

	if (MainCamera.ReferCameraX() > 0)
		MCPOX = Screen::SCREEN_X / 2 -
		abs(MainCamera.ReferPrintOutX(mapX * multiple / 2)) + flameCount;
	else
		MCPOX =
		abs(MainCamera.ReferPrintOutX(mapX * multiple / 2)) + flameCount;

	if (MainCamera.ReferCameraZ() > 0)
		MCPOZ = Screen::SCREEN_Z / 2 -
		abs(MainCamera.ReferPrintOutZ(mapZ * multiple / 2)) + flameCount;
	else
		MCPOZ =
		abs(MainCamera.ReferPrintOutZ(mapZ * multiple / 2)) + flameCount;


	if (flameCount >= mapX / 4)
		flameCount = fmod(flameCount, mapX / 4);

	flameCount += .05 * MainCamera.ReferZoomRatio();

	SetTrans(90);//�����x��������
	DrawExtendGraph((int)MCPOX, (int)MCPOZ,
		(int)(MCPOX + mapX * multiple),
		(int)(MCPOZ + mapZ * multiple),
		*PL.ReferNoiseHandle(), TRUE);
	DrawExtendGraph((int)(MCPOX - mapX * multiple),
		(int)MCPOZ,
		(int)(MCPOX),
		(int)(MCPOZ + mapX * multiple),
		*PL.ReferNoiseHandle(), TRUE);
	DrawExtendGraph((int)MCPOX,
		(int)(MCPOZ - mapZ * multiple),
		(int)(MCPOX + mapX * multiple), 
		(int)(MCPOZ),
		*PL.ReferNoiseHandle(), TRUE);
	DrawExtendGraph((int)(MCPOX - mapX * multiple),
		(int)(MCPOZ - mapZ * multiple),
		(int)(MCPOX),
		(int)(MCPOZ),
		*PL.ReferNoiseHandle(), TRUE);
	ResetTrans();//�`�惂�[�h�����Ƃɖ߂�
}

void IngameDataManagement::DrawMesh_Sea() {
	double mapX = PL.ReferMapX();
	double mapZ = PL.ReferMapZ();

	double MCPOX;
	double MCPOZ;

	MCPOX = Screen::SCREEN_X / 2 - MainCamera.ReferPrintOutX(mapX / 2);

	MCPOZ = Screen::SCREEN_Z / 2 - MainCamera.ReferPrintOutZ(mapZ / 2);
	
	double realX = fmod(MCPOX + flameCount, mapX / 2);
	double realZ = fmod(MCPOZ + flameCount, mapZ / 2);

	if (flameCount >= PL.ReferMapX())
		flameCount = fmod(flameCount, (double)PL.ReferMapX());

	flameCount += .05;

	SetTrans(90);//�����x��������
	DrawGraph((int)realX,
		(int)realZ,
		*PL.ReferNoiseHandle(), TRUE);
	DrawGraph((int)realX - PL.ReferMapX(),
		(int)realZ,
		*PL.ReferNoiseHandle(), TRUE);
	DrawGraph((int)realX,
		(int)realZ - PL.ReferMapZ(),
		*PL.ReferNoiseHandle(), TRUE);
	DrawGraph((int)realX - PL.ReferMapX(),
		(int)realZ - PL.ReferMapZ(),
		*PL.ReferNoiseHandle(), TRUE);
	ResetTrans();//�`�惂�[�h�����Ƃɖ߂�
}


void IngameDataManagement::DrawThisList(std::list<Effect> *effectList) {
	if (!effectList->empty())//���X�g�󋵂��m�F
		for (auto effect = effectList->begin();
			effect != effectList->end();
			effect++) {
		effect->Draw(MainCamera);//���΍��W�𗘗p���ĕ`��
	}
}

/*�D�̉��ɂ���G�t�F�N�g��`��*/
void IngameDataManagement::DrawEffectUnderShips() {
	DrawShipsShadow();//�D�̉e��`��
	/*���A���o*/
	SetTrans(90);//�����x��������
	DrawThisList(&bubbleList);
	SetTrans(80);//�����x��������
	DrawThisList(&rippleList);
	ResetTrans();//�`�惂�[�h�����Ƃɖ߂�
}

/*�D�̏�ɂ���G�t�F�N�g��`��*/
void IngameDataManagement::DrawEffectBeyondShips() {
	SetTrans(140);//�����x��������
	DrawThisList(&smokeList);
	SetTrans(210);//�����x��������
	DrawThisList(&explosionList);
	ResetTrans();//�`�惂�[�h�����Ƃɖ߂�
}

void IngameDataManagement::DrawPointOfImpact() {
	auto ship = alliesFleet.begin();

//	if (!ship->fireDataFigureUp.ReferLockOn())//���b�N���g�����ɗ����n�_��`�悵�܂���
		ship->ShowMePointOfImpact(MainCamera);//�g��Ȃ����ɕ`�悷��
}

void IngameDataManagement::DrawHPBar() {
	DrawBarForATeam(alliesFleet,true);
	DrawBarForATeam(enemyFleet,false);
}

void IngameDataManagement::DrawBarForATeam(std::vector<ShipMain> shipList, bool left) {
	if (!shipList.empty()) {
		int count = 0;

		for (auto ship = shipList.begin();
			ship != shipList.end();
			ship++) {

			Coordinate2D<double> coord;
			coord.x = left ? BUTTON_POSITION_INGAME::SHIP_MARK_LEFT_X_COORD
				: BUTTON_POSITION_INGAME::SHIP_MARK_RIGHT_X_COORD;

			coord.z = BUTTON_POSITION_INGAME::SHIP_MARK_NEXT_Z_COORD*(count + 1);

			UI.DrawHPBar(coord, ship->ReferHP(),ship->ReferMaxHP());
			
			count++;
		}
	}
}


void IngameDataManagement::DrawAmmo() {
	if (!shellList.empty())//���X�g�󋵂��m�F
	for (auto shell = shellList.begin();
		shell != shellList.end();
		shell++) {
		shell->Draw(MainCamera);//���΍��W�𗘗p���ĕ`�悷��
	}
}

//�ǂݍ��ݒ����
void IngameDataManagement::DrawLoading() {
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	unsigned int Cr;
	Cr = GetColor(255, 255, 255);
	DxLib::SetFontSize(30);

	DrawString((int)(0.8*Screen::SCREEN_X),
		(int)(0.8*Screen::SCREEN_Z), "LOADING...", Cr);

	DxLib::ScreenFlip();
}

void IngameDataManagement::DrawLoading(int num) {
	ClearDrawScreen();

	unsigned int Cr;
	Cr = GetColor(255, 255, 255);
	DxLib::SetFontSize(30);

	DrawString((int)(0.8*Screen::SCREEN_X),
		(int)(0.8*Screen::SCREEN_Z), "LOADING...", Cr);
	DrawFormatString((int)(0.8*Screen::SCREEN_X),
		(int)(0.85*Screen::SCREEN_Z), Cr, "%d/%d", num, asyncLoadNum);
	DrawBox(0, (int)(0.9*Screen::SCREEN_Z),
		(int)((double)num / (double)asyncLoadNum * Screen::SCREEN_X), 
		(int)(0.95*Screen::SCREEN_Z),
		Cr, TRUE);

	DxLib::ScreenFlip();
}

bool IngameDataManagement::FormationBoard() {
	while (1) {
		if (ProcessMessage() == -1)//�E�B���h����鎞�������I���ł���悤�ɂ���
			break;

		int ans = CommandSerial::NONE_COMMAND;//�I�[�_�[�����

		if (CUI.ReferClickable())
			ans = CUI.CheckChoice();

		CheckErrorList();

		if (ans == GAME_START) {//�����Q�[���X�^�[�g����������
			int flagShipCount = teamA[flagShipNum].ReferNumber();
			if (flagShipCount != 0 &&//�����ɗ��`�[���͋�łȂ����
				teamBCount != 0) {
				break;
			}
			if (teamACount == 0 || teamBCount == 0)
			{
				msgList.push_back(Fleet_Empty());
			}
			if (flagShipCount == 0 && teamACount != 0 && teamBCount != 0) {
				msgList.push_back(FlagShip_Empty());
			}
		}

		if (ans == CommandSerial::EXIT_IN_FORMATION) {
			shouldIContinue = false;
			return false;
		}

		if (ans >= CommandSerial::SELECT_IN_FORMATION &&
			ans < CommandSerial::SELECT_IN_FORMATION + 16) {
			CUI.SetClickTime();//�������Ԃ��L�^����
			ans -= 60;//�ԍ����L�^����
			if (ans % 2 == 0) {
				if (ans >= 8 && teamBCount < maxCountInATeam) {
					teamBCount++;
					teamB[(ans - 8) / 2].Plus();
				}
				if (ans < 8 && teamACount < maxCountInATeam) {
					teamACount++;
					teamA[ans / 2].Plus();
				}
			}
			if (ans % 2 == 1) {
				if (ans >= 8 && teamBCount > 0) {
					if (teamB[(ans - 8) / 2].ReferNumber() > 0) {
						teamBCount--;
						teamB[(ans - 8) / 2].Minus();
					}
				}
				if (ans < 8 && teamACount > 0) {
					if (teamA[ans / 2].ReferNumber() > 0) {
						teamACount--;
						teamA[ans / 2].Minus();
					}
				}
			}
		}

		if (ans == CommandSerial::RANDOM_FORMATION) {
			CUI.SetClickTime();
			ClearTeam();//�����_���O�ɃN���A����
			int temp = rand() % 9 + 1;
			SetRandom(temp, 0, true, &teamACount);
			SetRandom(temp, 0, false, &teamBCount);
		}

		if (ans == CommandSerial::CLEAR_FORMATION) {
			CUI.SetClickTime();
			ClearTeam();
		}

		/*�t���O�V�[�v�I�𕔕�*/
		Coordinate2D<int> mousePos;
		if(GetMouseInput() == MOUSE_INPUT_LEFT)
			GetMousePoint(&mousePos.x, &mousePos.z);
		for (int i = 0; i < 4; i++) {
			if (teamA[i].Clicked({ 70,100 + 120 * i }, mousePos)) {
				int tempF = i;
				for (int j = 0; j < 4; j++) {
					if (tempF == j)continue;
					if (teamA[j].ReferFlag())
						teamA[j].ChangeFlagStatus();
				}
				flagShipNum = i;
			}
		}

		DrawFormationBoard();
	}
	return true;
}

bool IngameDataManagement::RegisterTeam() {
	int count = 0;
	Coordinate<double> coord{ -500, -10, 200 };

	for (int i = 0; i < teamA[flagShipNum].ReferNumber(); i++) {
		alliesFleet.push_back(ShipMain());
		auto ship = alliesFleet.end();
		ship--;

		coord.x -= 300;//��͂̊Ԃ̊Ԋu�����

		if (!GetShipDataFromFile(coord, 0,
			&*ship, teamA[flagShipNum].ship.ReferShipType(),
			count)) {
			return false;
		}
		count++;
	}

	for (int i = 0; i < 4; i++) {
		if (i == flagShipNum)
			continue;
		for (int j = 0; j < teamA[i].ReferNumber(); j++) {

			alliesFleet.push_back(ShipMain());
			auto ship = alliesFleet.end();
			ship--;
			coord.x -= 300;//��͂̊Ԃ̊Ԋu�����

			if (!GetShipDataFromFile(coord, 0,
				&*ship, teamA[i].ship.ReferShipType(),
				count)) {//�t���O�V�[�v�̃^�C�v�̈ʒu�������
				return false;
			}
			count++;
		}
	}

	coord = { 3000,-10,1800 };
	count = 11;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < teamB[i].ReferNumber(); j++) {

			enemyFleet.push_back(ShipMain());
			auto ship = enemyFleet.end();
			ship--;
			coord.x += 300;//��͂̊Ԃ̊Ԋu�����

			if (!GetShipDataFromFile(coord, MathAndPhysics::PI,
				&*ship, teamB[i].ship.ReferShipType(), 
				count)) {
				return false;
			}
			count++;
		}
	}
	return true;
}

bool IngameDataManagement::GetShipDataFromFile(Coordinate<double> Coord,
	double radian, ShipMain *ship, int shipType,int num) {
	ship->SetCoord(Coord);
	ship->SetRadianOnZ(radian);
	if (!ship->InifThisShip(&PL, ET, &SL, shipType,
		num)) {
		DrawString(10, 10, "�t�@�C���ǂݍ��ގ��s", GetColor(255, 255, 255));
		DxLib::ScreenFlip();
		WaitKey();
		return false;
	}
	return true;
}

void IngameDataManagement::DrawFormationBoard() {
	SetDrawScreen(DX_SCREEN_BACK);

	ClearDrawScreen();

	DrawExtendGraph(0, 0,
		Screen::SCREEN_X, Screen::SCREEN_Z,
		*formationBoard[FormationBoard::FB_BACKGROUND], FALSE);

	teamA[0].DrawCard(Coordinate2D<double>{70, 100});
	teamA[1].DrawCard(Coordinate2D<double>{70, 220});
	teamA[2].DrawCard(Coordinate2D<double>{70, 340});
	teamA[3].DrawCard(Coordinate2D<double>{70, 460});

	teamB[0].DrawCard(Coordinate2D<double>{710, 100});
	teamB[1].DrawCard(Coordinate2D<double>{710, 220});
	teamB[2].DrawCard(Coordinate2D<double>{710, 340});
	teamB[3].DrawCard(Coordinate2D<double>{710, 460});

	SetFontSize(35);
	DrawFormatString(300, 16, GetColor(255, 255, 255),
		"����%2d��(10�ǂ܂�)",teamACount);
	DrawFormatString(670, 16, GetColor(255, 255, 255),
		"����%2d��(10�ǂ܂�)", teamBCount);

	CUI.Draw();

	Coordinate2D<int> mousePos;

	GetMousePoint(&mousePos.x, &mousePos.z);

	for (int i = 0; i < 4; i++) {
		if (teamA[i].CheckMousePos({ 70,100 + 120 * i }, mousePos)) {
			teamA[i].DrawBar(mousePos);
		}
		if (teamB[i].CheckMousePos({ 710,100 + 120 * i }, mousePos)) {
			teamB[i].DrawBar(mousePos);
		}
	}

	DrawErrorList();

	DxLib::ScreenFlip();
}

bool IngameDataManagement::InifFormationBoard() {
	for (int i = 0; i < 4; i++) {
		teamA.push_back(ShipCard(PL.ReferFormationBoardHandle(FormationBoard::FB_SHIP_CARD),
			PL.ReferFantasyNumber()));
		teamB.push_back(ShipCard(PL.ReferFormationBoardHandle(FormationBoard::FB_SHIP_CARD),
			PL.ReferFantasyNumber()));
	}
	
	if (!teamA[0].ship.InifThisShip(&PL, ET, &SL, 4000, 1)) return false;
	if (!teamA[1].ship.InifThisShip(&PL, ET, &SL, 5000, 1)) return false;
	if (!teamA[2].ship.InifThisShip(&PL, ET, &SL, 4001, 1)) return false;
	if (!teamA[3].ship.InifThisShip(&PL, ET, &SL, 5001, 1)) return false;

	SetRandom(6, 0, true, &teamACount);//�����_���ɐ�������

	if (!teamB[0].ship.InifThisShip(&PL, ET, &SL, 4000, 1)) return false;
	if (!teamB[1].ship.InifThisShip(&PL, ET, &SL, 5000, 1)) return false;
	if (!teamB[2].ship.InifThisShip(&PL, ET, &SL, 4001, 1)) return false;
	if (!teamB[3].ship.InifThisShip(&PL, ET, &SL, 5001, 1)) return false;

	SetRandom(6, 0, false, &teamBCount);//�����_���ɐ�������

	teamA[flagShipNum].SetFlag();
	return true;
}

bool IngameDataManagement::FreeFormationBoard() {//�t���[������Ƀ��j���[��ʏ��Ԃɐݒu����
	
	if (ProcessMessage() == 0 && shouldIContinue)
		if (!RegisterTeam())
			return false;
	
	for (int i = 0; i < 4; i++) {
		teamA[i].ship.DestroyMemory();
		teamB[i].ship.DestroyMemory();
	}

	teamA.clear();
	teamB.clear();
	std::vector<ShipCard>().swap(teamA);
	std::vector<ShipCard>().swap(teamB);

	if (ProcessMessage() == 0 && shouldIContinue) {
		alliesFleet[0].SetControled();//�F�R�͑��̈�Ԃ̑��쌠�����
		enemyFlagShip = &*enemyFleet.begin();
		CUI.InifShipList(&enemyFleet, false);
		CUI.InifShipList(&alliesFleet, true);
	}

	ClearErrorList();

	CUI.CloseFormationMenu();
	FC.Reset();

	return true;
}

void IngameDataManagement::DrawStatisticBoard2() {
	rewind(stdin);
	statisticBoardData.GetDis(alliesFleet[0].ReferDistanceMoved()*
		MathAndPhysics::Change_Distance/3600);
	statisticBoardData.Read(win);
	FC.Reset();
	while (ProcessMessage() == 0) {
		if (statisticBoardData.Update())
			break;
		FC.Wait();
	}
}

/****************************************************/
/*                     �e�X�g�@                     */
/****************************************************/
void IngameDataManagement::TEST_DRAW() {
	unsigned int Cr;
	Cr = GetColor(255, 255, 255);
	auto ship = alliesFleet.begin();
	//ship->TestDraw(MainCamera.ReferRealCameraX(),
	//	MainCamera.ReferRealCameraZ());
	char CharNum[255];

	DxLib::SetFontSize(15);
	ChangeFont("�c�e�f����Δ��");

	_gcvt_s(CharNum, ship->ReferSpeedOnZ() * 100, 10);
	DrawString(60, 10, "Speed", Cr);
	DrawString(110, 10, CharNum, Cr);
	if (ship->ReferReturnOn()) {
		DrawString(60, 30, "Alignment On", Cr);
	}
	else {
		DrawString(60, 30, "Alignment Off", Cr);
	}
	_gcvt_s(CharNum, ship->ReferChangingRadian(), 10);
	DrawString(60, 50, "Radian(C)", Cr);
	DrawString(150, 50, CharNum, Cr);
	_gcvt_s(CharNum, ship->ReferCoordX(), 10);
	DrawString(60, 70, "X", Cr);
	DrawString(80, 70, CharNum, Cr);
	_gcvt_s(CharNum, ship->ReferCoordZ(), 10);
	DrawString(60, 90, "Z", Cr);
	DrawString(80, 90, CharNum, Cr);
	_gcvt_s(CharNum, ship->ReferRadianOnZ(), 10);
	DrawString(60, 110, "Radian(R)", Cr);
	DrawString(150, 110, CharNum, Cr);
	_gcvt_s(CharNum, cos(ship->ReferRadianOnZ()), 10);
	DrawString(60, 130, "Cos", Cr);
	DrawString(100, 130, CharNum, Cr);
	_gcvt_s(CharNum, sin(ship->ReferRadianOnZ()), 10);
	DrawString(60, 150, "Sin", Cr);
	DrawString(100, 150, CharNum, Cr);
	_gcvt_s(CharNum, ship->ReferOutPutRate(), 10);
	DrawString(60, 170, "OutPutRate", Cr);
	DrawString(170, 170, CharNum, Cr);
	DrawString(60, 190, "FPS:", Cr);
	DrawFormatString(100, 190, Cr, "%2.3f", FC.ReferFPS());
}

void IngameDataManagement::EndTheGame() {
	showLock = false;
	if (sinkingShip.empty())
		GameOver = true;
}

/****************************************************/
/*                   �R���g���[��                   */
/****************************************************/
/*�R�}���h���󂯎���āA�V���Ȃ��̂𐶐�����*/
void IngameDataManagement::Control() {
	int answer;

	answer = CUI.CheckChoice();

	/*�d���N���b�N��h�~*/
	if (answer == CommandSerial::INCREASE_OUTPUT ||
		answer == CommandSerial::DECREASE_OUTPUT ||
		answer == CommandSerial::MENU ||
		answer == CommandSerial::TURN_RETURN||
		answer == CommandSerial::CHANGE_CAMERA ||
		(answer >= CommandSerial::SELECT && answer < CommandSerial::ZOOM_IN)||
		answer == CommandSerial::AUTO_FIRE)
		if (!CUI.ReferClickable())
			return;

	/*�}�E�X�ŃR�}���h���󂯂Ȃ������ꍇ�̓L�[�{�[�h����R�}���h���󂯎��*/
	if (answer == CommandSerial::NONE_COMMAND)
		answer = CT.GetCommand();

	/*�R�}���h���Ȃ���΂��̊֐����I������*/
	if (answer == CommandSerial::NONE_COMMAND)
		return;

	auto ship = alliesFleet.begin();

	/*�D�𑀍�*/
	if (ship->ReferAlive()) {
		ship->ControlThisShip(answer);
	}

	CUI.SetClickTime();//�N���b�N�������Ԃ��L�^����
	
	/*select�ȑO�̃R�}���h�̏���*/
	if (answer < CommandSerial::SELECT) {
		switch (answer) {
		case CommandSerial::SHOOT:if(alliesFleet[0].ReferAlive()) 
			TestShoot(&alliesFleet[0],true); break;/*�ˌ�*/
		case CommandSerial::MENU:CUI.LetMeSeeMenu();break;/*���j���[��ԕύX*/
		case CommandSerial::TEST_VIEW_ON:TEST_SHOW_ON = !TEST_SHOW_ON; break;	/*�e�X�g�r���[*/
		case CommandSerial::EXIT:GameOver = true; break;	/*�Q�[���I��*/
		case CommandSerial::CHANGE_CAMERA:showLock = !showLock; break;
		case CommandSerial::AUTO_FIRE:autoFire = !autoFire; break;
		}
	}

	else if (answer >= CommandSerial::ZOOM_IN) {
		switch (answer) {
		case CommandSerial::ZOOM_IN:MainCamera.ZoomIn(); break;
		case CommandSerial::ZOOM_OUT:MainCamera.ZoomOut(); break;
		case CommandSerial::GET_DAMAGE_TEST:alliesFleet[0].SufferDamage(5);
		}
	}

	else {
		ship->fireDataFigureUp.SetNumber(answer -
			CommandSerial::SELECT - CommandSerial::SELECT_RANGE);//�^�[�Q�b�g��ݒu����
		if (enemyFleet[answer -
			CommandSerial::SELECT - CommandSerial::SELECT_RANGE].ReferAlive()) {
			ship->fireDataFigureUp.LockOn_Switch();//���b�N��Ԃ�ύX
			ship->ResetReviseData();//�C���f�[�^�����Z�b�g
			CUI.SetShootMenu(ship->fireDataFigureUp.ReferLockOn());//�t�h��ύX
			autoFire = false;//�����ˌ��𖳌���
		}
	}
}

/****************************************************/
/*                     �ړ��֘A                     */
/****************************************************/
void IngameDataManagement::MoveAll() {
	CheckShipsStatus();//�D�̏�Ԃ��X�V����
	MoveShips();//�D���ړ�����
	MoveEffects();//�G�t�F�N�g���ړ�����
	MoveAmmo();//�e���ړ�����

	SimpleHitDecision();//�e�ƑD�̂����蔻��
	CrashDecision();//�D�̊Ԃ̂����蔻��
}

void IngameDataManagement::CheckShipListStatus(std::vector<ShipMain> *shipList) {
	if (!shipList->empty())
		for (auto ship = shipList->begin();
			ship != shipList->end();
			ship++) {
		ship->Update();//��ԍX�V
	}
}
//�D�̏�Ԃ��X�V����
void IngameDataManagement::CheckShipsStatus() {
	CheckShipListStatus(&alliesFleet);
	CheckShipListStatus(&enemyFleet);
}

/*���N���X���g���Ĉړ�����*/
void IngameDataManagement::MoveShips() {
	if(!alliesFleet.empty())//���X�g�m�F
	for (auto ship = alliesFleet.begin();
		ship != alliesFleet.end();
		ship++) {
		if(ship->ReferAlive())
		ship->Move();//�F�R�̑D���ړ�����
	}
	if(!enemyFleet.empty())//���X�g�m�F
	for (auto ship = enemyFleet.begin();
		ship != enemyFleet.end();
		ship++) {
		if (ship->ReferAlive())
		ship->Move();//�G�̑D���ړ�����
	}
}

void IngameDataManagement::MoveEffect(std::list<Effect> *effectList) {
	if (!effectList->empty())//���X�g�m�F
		for (auto effect = effectList->begin();
			effect != effectList->end();
			effect++) {
		effect->Move();//���A��D�̌��Ɉړ�����
	}
}

void IngameDataManagement::MoveEffects() {
	MoveEffect(&smokeList);
	MoveEffect(&bubbleList);
}

void IngameDataManagement::MoveAmmo() {
	if (!shellList.empty())//���X�g�m�F
		for (auto shell = shellList.begin();
			shell != shellList.end();
			shell++) {
		shell->Move();
	}
}

/****************************************************/
/*                    �f�[�^�֘A                    */
/****************************************************/
bool IngameDataManagement::Inif() {

	long long startTime = GetTickCount();

	SetUseASyncLoadFlag(TRUE);//�񓯊��ǂݍ��݂�L����
	PL.InifForGame();//�摜���[�_�[������
	SL.Inif();//�������[�_�[������
	SetUseASyncLoadFlag(FALSE);//�񓯊��ǂݍ��݂𖳌���

	SetDrawScreen(DX_SCREEN_BACK);

	asyncLoadNum = GetASyncLoadNum();

	while (ProcessMessage() == 0) {
		DrawLoading(asyncLoadNum - GetASyncLoadNum());
		if(GetASyncLoadNum() ==0)
			break;
		if (GetTickCount() - startTime > TIME_NEEDED::ONE_MINUTE ) {
			return false;
		}
	}

	Sleep(10);
	SetFontThickness(6);
	PL.GetGraphSizeForGame();//�摜�̑傫����ݒu����
	UI.InifUI(&PL);//UI������
	ET.InifEffectTemplate(&PL);//�G�t�F�N�g�e���v���[�g������
	CT.Inif(&SL);//�L�[�{�[�h�R���g���[���[������
	CUI.IngameInif(&PL,&SL);//�}�E�X�R���g���[���[������
	
	statisticBoardData.Inif(PL.ReferStatisticBoardHandle(0));/*���v�{�[�h�[�̏�����*/
	if (!InifFormationBoard()) {//�Ґ���ʂ�����������
		DrawString(30, 30, "�t�@�C���ǂݍ��ގ��s\n�^�C�g���ɖ߂�܂�",
			GetColor(255, 255, 255));
		DxLib::ScreenFlip();
		WaitKey();
		return false;
	}

	for (int i = 0; i < FormationBoard::FB_NUM; i++) {
		formationBoard[i] = PL.ReferFormationBoardHandle(i);
	}

	CUI.SetFormationMenuStatus();/*�����̓e�X�g�o�[�W����*/

	return true;
}

/*�g�������������������*/
void IngameDataManagement::Free() {
	PL.FREE_FOR_GAME();//���I������������A���������̃f�[�^���폜
	SL.FreeAll();//���I������������A���������̃f�[�^���폜
	CUI.Free();//�t�h�����̓��I���������������
	DestroyShips();//�D�����p�������I���������������

	using namespace std;

	/*�W�����C�u�������������*/
	/*vector���*/
	alliesFleet.clear();
	vector<ShipMain>().swap(alliesFleet);
	enemyFleet.clear();
	vector<ShipMain>().swap(enemyFleet);

	/*���X�g���*/
	shellList.clear();
	bombList.clear();
	torpedoList.clear();
	bubbleList.clear();
	smokeList.clear();
	explosionList.clear();
	rippleList.clear();
	normalEffectList.clear();
	msgList.clear();
}

/*�������Ԓ��������̂�����*/
void IngameDataManagement::DeleteUseless() {
	CheckErrorList();
	DeleteUselessEffect();//���Ԑ؂�G�t�F�N�g������
	DeleteUselessAmmo();//�C�ɗ������C�e������
	RemoveDestroyedShip();//���񂾑D�𒾂ރ��X�g�ɒǉ�����
}

void IngameDataManagement::DestroyShips() {
	DestroyThisTeam(&alliesFleet);//�F�R�͑��̃��������
	DestroyThisTeam(&enemyFleet);//�G�͑��̃��������
}

void IngameDataManagement::DestroyThisTeam(std::vector<ShipMain> *shipList) {
	if (!shipList->empty()) {
		for (auto ship = shipList->begin();
			ship != shipList->end();
			ship++) {
			ship->DestroyMemory();//���������
		}
	}
}

void IngameDataManagement::DestroyThisTeam(std::list<ShipMain> *shipList) {
	if (!shipList->empty()) {
		for (auto ship = shipList->begin();
			ship != shipList->end();
			ship++) {
			ship->DestroyMemory();//���������
		}
	}
}

void IngameDataManagement::CheckTeamStatus() {
	/*�F�R�̏�Ԃ��m�F����*/
	CheckAlliesStatus();
	/*�G�̏�Ԃ��m�F����*/
	CheckEnemyStatus();
}

void IngameDataManagement::CheckAlliesStatus() {
	if (alliesFleet[0].ReferAlive()) {//�t���O�V�[�v�����ނ܂Ő킦��
		for (auto ship = alliesFleet.begin();
			ship != alliesFleet.end();
			ship++) {
			if (ship->ReferAlive())
				return;
		}
	}
	win = false;//�F�R�͑��͑S�ł��ꂽ�畉���ł�
	GameEnd = true;/*�I����Ԃ�ݒu*/
}

void IngameDataManagement::CheckEnemyStatus() {
	if (enemyFleet[0].ReferAlive())
		for (auto ship = enemyFleet.begin();
			ship != enemyFleet.end();
			ship++) {
		if (ship->ReferAlive())
			return;
	}
	win = true;//�G�R�͑��͑S�ł��ꂽ�畉���ł�
	GameEnd = true;/*�I����Ԃ�ݒu*/
}

void IngameDataManagement::CheckAndPlaySound() {
	auto ship = alliesFleet.begin();
	ship->CheckAndPlaySound();//�����̑D�Ɋ֘A���鉹�𗬂�
}

/****************************************************/
/*                  �G�t�F�N�g�֘A                  */
/****************************************************/
void IngameDataManagement::GetNewEffect() {
	/*�i�s���̐��A����*/
	NewEffectForShips(alliesFleet);
	NewEffectForShips(enemyFleet);
}

void IngameDataManagement::NewEffectForShips(std::vector<ShipMain> shipList) {
	/*�i�s���̐��A����*/
	if (!shipList.empty()) {//���X�g�m�F
		for (auto ship = shipList.begin();
			ship != shipList.end();
			ship++) {
			if(ship->ReferAlive())
			if (abs(ship->ReferSpeedOnZ()) > 0.05 && rand() % 30 == 0
				&& rand() % 100 > ship->ReferSpeedOnZ() * 10) {//���̑��x������΁A�m���Ő�������
				//for(int i = 0; i < ship->ReferBubbleCount(); i++)
//				if (counter % 20 == 0)
				for (int i = 0; i < ship->ReferBubblePointCount(); i++)
					//���ׂẴG�t�F�N�g�|�C���g����G�t�F�N�g�𐶐�����
					bubbleList.push_back(ship->NewBubble(i));//���X�g�̖��ɒǉ�����
			}
		}
		for (auto ship = shipList.begin();
			ship != shipList.end();
			ship++) {
			if (ship->ReferAlive())
			if (ship->ReferOutPutRate() != 0) {
				if ((rand() % 8 < abs(ship->ReferSpeedOnZ()) * 10)
					&& counter % 10 == 0) {//�m���Ő�������
					//for(int i = 0; i < ship->ReferSmokeCount(); i++)
					for (int i = 0; i < ship->ReferSmokePointCount(); i++)
						smokeList.push_back(ship->NewSmoke(i));//���X�g�̍Ō�ɒǉ�����
				}
			}
		}
	}
}

/*�V���������G�t�F�N�g�𐶐�����*/
void IngameDataManagement::NewExplosion(Coordinate2D<double> Coord) {
	/*�摜�̌����������_���ł�*/
	double radian = (double)(rand() % 180) * 1.0 / 180.0 * MathAndPhysics::PI;
	Effect effect(TypeOfEffect::EXPLOSION,
		false, 500, radian, 0, 0, 0, Coord.x, Coord.z,
		PL.ReferEffectList(TypeOfEffect::EXPLOSION), true,
		0.15 * ((double)(rand()%4) + 1), 1.05);

	explosionList.push_back(effect);//�������ꂽ���̂����X�g�̌��ɒǉ�����
}

void IngameDataManagement::NewRipple(double coordX, double coordZ) {
	Effect ripple(TypeOfEffect::RIPPLE,
		false, 600, 0, 0, 0, 0, coordX, coordZ,
		PL.ReferEffectList(TypeOfEffect::RIPPLE), true, 0.45, 1.09);
	//�C�e�����������Ƃ���Ɉ�b������G�t�F�N�g�𐶐�����

	rippleList.push_back(ripple);//���������G�t�F�N�g�����X�g�̖��ɒǉ�����
}


void IngameDataManagement::DeleteUselessEffect() {
	CheckAndCleanThisEffectList(&bubbleList);//���A���X�g�̒��̂���Ȃ����̂�����
	CheckAndCleanThisEffectList(&smokeList);//�_���X�g�̒��̂���Ȃ����̂�����
	CheckAndCleanThisEffectList(&rippleList);//
	CheckAndCleanThisEffectList(&explosionList);//�������X�g���N���A
}

void IngameDataManagement::CheckAndCleanThisEffectList(std::list<Effect> *effectList) {
	if (!effectList->empty())//�󂫂̏ꍇ�̓X�L�b�v����
		for (auto effect = effectList->begin();
			effect != effectList->end();
			effect++) {
		effect->Update();//��ԍX�V
		if (effect->ReferTimeUp()) {//���Ԓ�������
			effect = effectList->erase(effect);//���X�g����폜
		}
		if (effectList->empty() || effect == effectList->end())
			//�������X�g�̖��ɂȂ�����J��Ԃ����I������
			break;
	}
}

/****************************************************/
/*                     �ˌ��֘A                     */
/****************************************************/

void IngameDataManagement::TestShoot(ShipMain *ship,bool me) {
	FiringData FD;
	//�Q�[�����ł́A�G���ˌ����鎞�ɑS��true�ɂ���
	FD.isThisMainWeapon = true;
	for (int i = 0; i < 8; i++) {
		FD.selected[i] = true;//�����I��
	}
	InputNewAmmo(ship, FD, me);//���X�g�ɐV�����e��ǉ�����
}

void IngameDataManagement::LockAndRefresh() {
	counter++;
	if (counter >= 60)//60�t���[�����ƂɃ��Z�b�g����
		counter = counter % 60;

	shouldIRender = counter % 60 == 0 ? true : false;//���b��񃌃��_�[����

	CheckThisTeamLock(&alliesFleet, enemyFleet);
	CheckThisTeamLock(&enemyFleet, alliesFleet);

}

void IngameDataManagement::CheckThisTeamLock(std::vector<ShipMain> *shipList,
	std::vector<ShipMain> enemyList) {
	if (!shipList->empty() && !enemyList.empty()) {
		for (auto ship = shipList->begin();
			ship != shipList->end();
			ship++) {
			if(ship->ReferAlive())
			if (ship->fireDataFigureUp.ReferLockOn()) {//�������b�N�͎g���Ă����
				ship->LockAndAim(&enemyList[ship->fireDataFigureUp.ReferTarget()],
					shouldIRender);//�I�񂾓G�����b�N����
				//�D�Q��ڕW�ɂ��ă��b�N����
			}
		}
	}
}

void IngameDataManagement::InputNewAmmo(ShipMain *SM, FiringData FD ,bool me) {
	int weaponAmount = SM->ReferWeaponCount(FD.isThisMainWeapon);//�I�񂾕�����m�F����
	for (int i = 0; i < weaponAmount; i++) {
		if (FD.selected[i]) {
			if (SM->IsThisOneUsable(i, FD.isThisMainWeapon)) {//�ˌ����\�ł����
				shellList.push_back(SM->Shoot(i,FD.isThisMainWeapon));//�V�����e��ǉ�����
				if (me)
					statisticBoardData.CountShoot();
			}
		}
	}
}

void IngameDataManagement::DeleteUselessAmmo() {
	if (!shellList.empty())//���X�g�m�F
		for (auto shell = shellList.begin();
			shell != shellList.end();
			shell++) {
		if (shell->FallIntoWater()|| !shell->ReferUsable()) {
			if (shell->FallIntoWater())//�����e�������ɗ���������
				NewRipple(shell->ReferCoordX(), shell->ReferCoordZ());//�e�������O�ɐ��A�𐶐�����
			shell = shellList.erase(shell);//�e������
		}
		if (shellList.empty() || shell == shellList.end())
			break;
	}
}


/****************************************************/
/*                     �����蔻��                   */
/****************************************************/
void IngameDataManagement::CrashDecision() {
	if (!alliesFleet.empty() && !enemyFleet.empty()) {//���X�g�m�F
		for (auto ship1 = alliesFleet.begin();
			ship1 != alliesFleet.end();
			ship1++) {
			if (ship1->ReferAlive())//�������Ԋm�F
			for (auto ship2 = enemyFleet.begin();
				ship2 != enemyFleet.end();
				ship2++) {
				if (ship2->ReferAlive())//�������Ԋm�F
				if (PointsToCollisionbox(&*ship1, &*ship2)) {
					ship1->Unmove(); ship1->ResetStatus();//�D���~����
					ship2->Unmove(); ship2->ResetStatus();
				}
			}
		}
		//�G���m�̃`�F�b�N
		for (auto ship1 = enemyFleet.begin();
			ship1 != enemyFleet.end();
			ship1++) {
			if (ship1->ReferAlive())//�������Ԋm�F
				for (auto ship2 = enemyFleet.begin();
					ship2 != enemyFleet.end();
					ship2++) {
				if (ship1 == ship2)//�����Ƃ̂����蔻������܂���
					continue;
				if (ship2 -> ReferAlive())//�������Ԋm�F
					if (PointsToCollisionbox(&*ship1, &*ship2)) {
						ship1->Unmove(); ship1->ResetStatus();//�D���~����
						ship2->Unmove(); ship2->ResetStatus();
					}
			}
		}
		//�F�R���m�̃`�F�b�N
		for (auto ship1 = alliesFleet.begin();
			ship1 != alliesFleet.end();
			ship1++) {
			if (ship1->ReferAlive())//�������Ԋm�F
				for (auto ship2 = alliesFleet.begin();
					ship2 != alliesFleet.end();
					ship2++) {
				if (ship1 == ship2)//�����Ƃ̂����蔻������܂���
					continue;
				if (ship2->ReferAlive())//�������Ԋm�F
					if (PointsToCollisionbox(&*ship1, &*ship2)) {
						ship1->Unmove(); ship1->ResetStatus();//�D���~����
						ship2->Unmove(); ship2->ResetStatus();
					}
			}
		}
	}
}

bool IngameDataManagement::PointsToCollisionbox(ShipMain *ship1, ShipMain *ship2) {
	for (int i = 0; i < ship1->ReferCollisionPointAmount(); i++) {
		if (ship2->ReferSinkingEnding())//�D�ɂ̏�Ԃ��m�F
			break;//���񂾂�I���

		Coordinate<double> temp;
		Coordinate2D<double> temp2D;
		temp = ship1->ReferCoord();//�D1�ɂ̏�Ԃ����
		temp2D = ship1->ReferCollisionPoint(i);//�D1�̂����蔻��_�����

		/*�{���̍��W���v�Z����*/
		temp.x = temp.x - cos(ship1->ReferRadianOnZ())
			* temp2D.x + sin(ship1->ReferRadianOnZ()) * temp2D.z;
		temp.z = temp.z - cos(ship1->ReferRadianOnZ())
			* temp2D.z - sin(ship1->ReferRadianOnZ()) * temp2D.x;
		temp.y = 5;
		
		//�_�ƒ����`�̂����蔻��֐��𗘗p���Čv�Z����
		if (crash3DtoPoint(ship2->ReferCoord(),
			temp, ship2->ReferShipCrashSize(),
			ship2->ReferRadianOnZ())) {
			return true;
		}
	}
	return false;
}

void IngameDataManagement::SimpleHitDecision() {
	if (!shellList.empty())//���X�g�󂫊m�F
		for (auto shell = shellList.begin();
			shell != shellList.end();
			shell++) {
		CheckThisTeamDecision(&alliesFleet, &*shell);//�F�R�̃`�F�b�N
		CheckThisTeamDecision(&enemyFleet, &*shell);//�G�̃`�F�b�N
	}
}



void IngameDataManagement::CheckThisTeamDecision(std::vector<ShipMain> *shipList,
	Ammo *shell) {
	if (!shipList->empty())//���X�g�󂫊m�F
		for (auto ship = shipList->begin();
			ship != shipList->end();
			ship++) {
		if (shell->ReferUsable())
			if (shell->ReferSerialNumber() != ship->ReferSerialNumber()) {
				//�e�ƑD�̂����蔻����s��
				if (crash3DtoPoint(ship->ReferCoord(),
					shell->ReferCoord(), ship->ReferShipCrashSize(),
					ship->ReferRadianOnZ()))
					if (ship->ReferAlive()) {
						//����������
						ship->SufferDamage((int)shell->ReferDamage());//�_���[�W��^����
						Coordinate2D<double> C2D = { shell->ReferCoordX(),
						shell->ReferCoordZ() };
						NewExplosion(C2D);//���������Ƃ���ɔ����G�t�F�N�g�𐶐�
						shell->Unusable();//�e���g���Ȃ��Ȃ�

						if (shell->ReferSerialNumber() == 0) {
							statisticBoardData.CountHit();//�q�b�g������
							statisticBoardData.CountDamage((int)shell->ReferDamage());//�_���[�W������
							if (!ship->ReferAlive()) {
								statisticBoardData.CountKilled();
							}
						}
						if (ship->ReferSerialNumber() == 0)
							statisticBoardData.CountDamageRec((int)shell->ReferDamage());

						/*���b�N���g���Ȃ����Ƀ��b�N����������*/
						if (!ship->ReferAlive())//���ޏ�Ԕ���
							if (alliesFleet[0].fireDataFigureUp.ReferTarget() ==
								//���b�N�̃^�[�Q�b�g�ł͂Ȃ���
								ship->ReferSerialNumber() - SELECT_RANGE - 1 &&
								//�G�ł���
								ship->ReferSerialNumber() > SELECT_RANGE) {
								if (alliesFleet[0].fireDataFigureUp.ReferLockOn() == true) {
									alliesFleet[0].fireDataFigureUp.LockOn_Switch();//���b�N��Ԃ�ύX
									ship->ResetReviseData();//�C���f�[�^�����Z�b�g	
									CUI.SetShootMenu(false);//�t�h��ύX
								}
						}
						return;
					}
			}
	}
}

void IngameDataManagement::RemoveDestroyedShip() {
	CheckTeamA(&alliesFleet);
	CheckTeamA(&enemyFleet);
}

/*���̊֐��͑D������������ɒ��މ��o�𐳏�ɍs�����߂ɍ��܂���*/
void IngameDataManagement::CheckTeamA(std::vector<ShipMain> *shipList) {
	if(!shipList->empty()){//���X�g�󂫊m�F
		for (auto ship = shipList->begin();
			ship != shipList->end();
			ship++) {
			if (!ship->ReferAlive()&&!ship->ReferInList()) {
				/*�����̓A�h���X�����X�g�ɒǉ������݂����B�B�B*/
				sinkingShip.push_back(*ship);//����ł��郊�X�g�ɒǉ�����
				ship->PutInList();
			}
		}
	}
}

void IngameDataManagement::RemoveSinkedShip() {
	if (!sinkingShip.empty()) {
		for (auto ship = sinkingShip.begin();
			ship != sinkingShip.end();
			ship++) {
			if (ship->ReferSinkingEnding()) {
				/*�Ɨ��ȃ��������g���Ă��܂���AdestroyMemory�͂���Ȃ�*/
				ship = sinkingShip.erase(ship);
			}
			if (sinkingShip.empty() || ship == sinkingShip.end())
				break;
		}
	}
}

/*�G���[����*/
void IngameDataManagement::DrawErrorList() {
	if (!msgList.empty())
		for (auto msg = msgList.begin();
			msg != msgList.end();
			msg++) {
		msg->DrawMessege();
	}
}

void IngameDataManagement::CheckErrorList() {
	if(!msgList.empty())
	for (auto msg = msgList.begin();
		msg != msgList.end();
		msg++) {
		if (msg->ReferDestroy()) {
			msg = msgList.erase(msg);
			if (msgList.empty() || msg == msgList.end())
				break;
		}
	}
}

void IngameDataManagement::ClearErrorList() {
	msgList.empty();
}

void IngameDataManagement::SetRandom(int left, int num, bool teamA, int *teamCount) {
	int temp = 0;
	temp = rand() % left + 1;
	if (temp > maxCountInATeam / 3) {//���ނ̑D�̐��𑽂����ɂȂ�Ȃ��悤��
		temp /= 2;
	}
	if (left <= 2)//�Ō�c���������S��������
		temp = left;
	SetThisShipCount(teamCount, num, temp, teamA);
	left -= temp;
	if (left <= 0 || num + 1 >= 4)
		return;
	SetRandom(left, num + 1, teamA, teamCount);
}

void IngameDataManagement::ClearTeam() {
	for (int i = 0; i < 4; i++) {
		teamA[i].SetNumber(0);
		teamB[i].SetNumber(0);
	}
	teamACount = 0;
	teamBCount = 0;
}

bool IngameDataManagement::ShouldIDraw() {
	if (shouldIDraw)
		return true;
	if (!shouldIDraw && haveIDrawed) {
		haveIDrawed = false;
		return true;
	}

	if (!shouldIDraw) {
		haveIDrawed = true;
		return false;
	}
	return false;
}