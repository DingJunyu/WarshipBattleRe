#include "IngameDataManagement.h"
/*�C�e���[�^���g���O�ɕK���e��̏󋵂��m�F���܂��I�I�I�I�I*/
using namespace DxLib;
IngameDataManagement::~IngameDataManagement()
{
}

/*���C�����[�g*/
void IngameDataManagement::Update() {
	SetDrawScreen(DX_SCREEN_BACK);//����ʂɕ`�悷��
	ClearDrawScreen();

	counter++;
	if (counter >= 60)//60�t���[�����ƂɃ��Z�b�g����
		counter = counter % 60;

	shouldIRender = counter % 20 == 0 ? true : false;//���b�O�񃌃��_�[����

	/*���b�N�e�X�g*/
	if (!alliesFleet.empty() && !enemyFleet.empty()) {
		auto ship1 = alliesFleet.begin();//���X�g�̈�Ԃ����
		if (ship1->fireDataFigureUp.ReferLockOn()) {//�������b�N�͎g���Ă����
			ship1->TestLock(&enemyFleet[ship1->fireDataFigureUp.ReferTarget()],
				shouldIRender);//�I�񂾓G�����b�N����
			//�D�Q��ڕW�ɂ��ă��b�N����
		}
	}
	/**************/

	MainCamera.GetXZ(ReferPlayerX(), ReferPlayerZ());//�J�������W���X�V
	Control();//�R�}���h���󂯎���āA�D�̏�Ԃ�ύX����
	LetFlagShipMove();//�G�̃t���O�V�[�v�𓮂���
	GetNewEffect();//�G�t�F�N�g�𐶐�����
	MoveAll();//�ړ��A��ԍX�V

	DeleteUseless();//����Ȃ����̂�����


	CheckAndPlaySound();
	DrawAll();//�S���X�V�������ʂ�`��
	FC.Wait();
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
void IngameDataManagement::LetFlagShipMove() {
	flagShipAI.LetUsGo(&enemyFleet[0], &alliesFleet[0]);
	enemyFleet[0].SetChangingDirect(flagShipAI.ReferRadianNeededNow());
	enemyFleet[0].SetEngineOutPutRate(flagShipAI.ReferSpeedInNeed());
}

void IngameDataManagement::LetEveryOneMove() {
	ControlThisList(&alliesFleet, &AI);
	ControlThisList(&enemyFleet, &AI);
}

void IngameDataManagement::ControlThisList(std::vector<ShipMain> *shipList,
	ArtificialIntelligence *AI) {	
	int num = 0;
	if (!shipList->empty() && shipList->begin()!=shipList->end()) {
		for (auto ship = shipList->begin();
			ship != shipList->end();
			ship++) {
			if (num == 0) {
				num++;
				continue;
			}
			auto prevShip = ship--;
			AI->Move(*ship, *prevShip);
			ship->SetChangingDirect(AI->ReferRadianNeededNow());
		}
	}
}

/****************************************************/
/*                     �`��֘A                     */
/****************************************************/
void IngameDataManagement::DrawAll() {
	auto ship = alliesFleet.begin();//�C�e���[�^�ő��삵�Ă���D�̃X�e�[�^�X�����

	/*�C��`��*/
	DrawSea();

	//�D�̉��̃G�t�F�N�g��`�悷��
	DrawEffectUnderShips();
	/*�D�̂����ɂ���UI��`�悷��*/
	UI.DrawUIUnderShip(ship->fireDataFigureUp.ReferLockOn(),
		enemyFleet[ship->fireDataFigureUp.ReferTarget()].ReferCoord2D_d(),
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
	UI.DrawUINeedInput(&*ship);//�D�̃X�e�[�^�X�Ɋ֌W������UI��`�悷��
	DrawShipsOnMiniMap();//�~�j�}�b�v��`��
	/*�N���b�N�\��UI*/
	CUI.Draw();//�{�^����`��
	CUI.DrawNeedInput(ship->fireDataFigureUp.ReferLockOn(), 
		ship->fireDataFigureUp.ReferTarget());//�������K�v�ȕ�����`��

	/*�e�X�g�C���t�H���[�V����*/
	if (TEST_SHOW_ON)
		TEST_DRAW();
	
	ScreenFlip();
}

/*�{�̂Ɖe�𕪂��ĕ`�悷��*/
void IngameDataManagement::DrawShips() {
	//�F�R��`�悷�� 
	//�R���g���[�����Ă���D���F�R�͑��̈�Ԃɂ���
	if (!alliesFleet.empty())//���X�g�󋵂��m�F
		for (auto mark = alliesFleet.begin();
			mark != alliesFleet.end(); mark++) {
		if (mark->ReferAlive()) {//�^�[�Q�b�g�̏�Ԃ��m�F
			if (mark == alliesFleet.begin())
				mark->Draw(MainCamera);//������������ʒ��S�ɉf��
			else
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
}

void IngameDataManagement::DrawShipsShadow() {
	//�F�R��`�悷�� 
	//�R���g���[�����Ă���D���F�R�͑��̈�Ԃɂ���
	if (!alliesFleet.empty())//���X�g�󋵂��m�F
		for (auto mark = alliesFleet.begin();
			mark != alliesFleet.end(); mark++) {
		if (mark->ReferAlive()) {//�^�[�Q�b�g�̏�Ԃ��m�F
			if (mark == alliesFleet.begin())
				mark->DrawShadow(MainCamera);//������������ʒ��S�ɉf��
			else
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

void IngameDataManagement::DrawThisList(std::list<Effect> *effectList) {
	if (!effectList->empty())//���X�g�󋵂��m�F
		for (auto effect = effectList->begin();
			effect != effectList->end();
			effect++) {
		effect->Draw((int)MainCamera.ReferRealCameraX(),
			(int)MainCamera.ReferRealCameraZ());//���΍��W�𗘗p���ĕ`��
	}
}

/*�D�̉��ɂ���G�t�F�N�g��`��*/
void IngameDataManagement::DrawEffectUnderShips() {
	DrawShipsShadow();//�D�̉e��`��
	/*���A���o*/
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 30);//�����x��������
	DrawThisList(&bubbleList);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 80);//�����x��������
	DrawThisList(&rippleList);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);//�`�惂�[�h�����Ƃɖ߂�
}

/*�D�̏�ɂ���G�t�F�N�g��`��*/
void IngameDataManagement::DrawEffectBeyondShips() {
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);//�����x��������
	DrawThisList(&smokeList);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);//�����x��������
	DrawThisList(&explosionList);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);//�`�惂�[�h�����Ƃɖ߂�
}

void IngameDataManagement::DrawPointOfImpact() {
	auto ship = alliesFleet.begin();

	if (!ship->fireDataFigureUp.ReferLockOn())//���b�N���g�����ɗ����n�_��`�悵�܂���
		ship->ShowMePointOfImpact(MainCamera);//�g��Ȃ����ɕ`�悷��
}

void IngameDataManagement::DrawAmmo() {
	if (!shellList.empty())//���X�g�󋵂��m�F
	for (auto shell = shellList.begin();
		shell != shellList.end();
		shell++) {
		shell->Draw((int)MainCamera.ReferRealCameraX(),
			(int)MainCamera.ReferRealCameraZ());//���΍��W�𗘗p���ĕ`�悷��
	}
}

//�ǂݍ��ݒ����
void IngameDataManagement::DrawLoading() {
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	unsigned int Cr;
	Cr = GetColor(255, 255, 255);
	SetFontSize(30);

	DrawString((int)(0.85*Screen::SCREEN_X),
		(int)(0.85*Screen::SCREEN_Z), "LOADING...", Cr);

	ScreenFlip();
}

/****************************************************/
/*                     �e�X�g�@                     */
/****************************************************/
void IngameDataManagement::TEST() {
	alliesFleet.push_back(ShipMain());//�V�����D�𐶐�����
	auto ship = alliesFleet.begin();//�C�e���[�^���g���Ď����̑D��I��
	//�����̕����͏������֐��ƃt�@�C���ǂݍ��݂͂܂��o���Ă��Ȃ�
	//�������֐��͉��̂悤�ɂ�����
	ship->SetMultiple(0.125);
	ship->InifThisShip(PL.ReferBattleCrusierHandle(4000), 
		PL.ReferBattleCrusierShadowHandle(4000), 4000, ET, &SL);//������
	ship->NewCoordX(640);//�V�������W��������
	ship->NewCoordZ(380);
	ship->NewCoordY(-10);
	ship->SetRadianOnZ(0);
	ship->SetLength(PL.ReferShipSizeX());//�T�C�Y��ݒ�
	ship->SetWidth(PL.ReferShipSizeZ());
	ship->TEST();//�e�X�g�֐������[�h����
	ship->SetSerialNumber(1);
	ship->SetWeaponTest(&PL);//��������[�h����


	for (int i = 0; i < 1; i++) {
		enemyFleet.push_back(ShipMain());//�e�X�g�p�G�D�𐶐�����
		auto enemyShip = enemyFleet.end();//�C�e���[�^�őD��I��
		enemyShip--;
		double radian = (double)(rand() % 180) / 180.0*MathAndPhysics::PI;

		enemyShip->SetMultiple(0.125);
		enemyShip->InifThisShip(PL.ReferBattleCrusierHandle(4000),
			PL.ReferBattleCrusierShadowHandle(4000), 4000, ET, &SL);
		enemyShip->NewCoordX(1200 + (rand() % 400) * i);
		enemyShip->NewCoordZ(400 +  (rand() % 400) * i);
		enemyShip->NewCoordY(-10);
		enemyShip->SetRadianOnZ(radian);
		enemyShip->SetLength(PL.ReferShipSizeX());
		enemyShip->SetWidth(PL.ReferShipSizeZ());
		enemyShip->TEST();
		enemyShip->SetSerialNumber(2);
		enemyShip->SetWeaponTest(&PL);
		enemyShip->SetEngineOutPutRate(true);
	}
}

void IngameDataManagement::TEST_DRAW() {
	unsigned int Cr;
	Cr = GetColor(255, 255, 255);
	auto ship = alliesFleet.begin();
	ship->TestDraw(MainCamera.ReferRealCameraX(),
		MainCamera.ReferRealCameraZ());
	char CharNum[255];

	SetFontSize(15);
	ChangeFont("HG��Я�M");

	_gcvt_s(CharNum, ship->ReferSpeedOnZ() * 100, 10);
	DrawString(10, 10, "Speed", Cr);
	DrawString(60, 10, CharNum, Cr);
	if (ship->ReferReturnOn()) {
		DrawString(10, 30, "Alignment On", Cr);
	}
	else {
		DrawString(10, 30, "Alignment Off", Cr);
	}
	_gcvt_s(CharNum, ship->ReferChangingRadian(), 10);
	DrawString(10, 50, "Radian(C)", Cr);
	DrawString(100, 50, CharNum, Cr);
	_gcvt_s(CharNum, ship->ReferCoordX(), 10);
	DrawString(10, 70, "X", Cr);
	DrawString(30, 70, CharNum, Cr);
	_gcvt_s(CharNum, ship->ReferCoordZ(), 10);
	DrawString(10, 90, "Z", Cr);
	DrawString(30, 90, CharNum, Cr);
	_gcvt_s(CharNum, ship->ReferRadianOnZ(), 10);
	DrawString(10, 110, "Radian(R)", Cr);
	DrawString(100, 110, CharNum, Cr);
	_gcvt_s(CharNum, cos(ship->ReferRadianOnZ()), 10);
	DrawString(10, 130, "Cos", Cr);
	DrawString(50, 130, CharNum, Cr);
	_gcvt_s(CharNum, sin(ship->ReferRadianOnZ()), 10);
	DrawString(10, 150, "Sin", Cr);
	DrawString(50, 150, CharNum, Cr);
	_gcvt_s(CharNum, ship->ReferOutPutRate(), 10);
	DrawString(10, 170, "OutPutRate", Cr);
	DrawString(110, 170, CharNum, Cr);
}

void IngameDataManagement::TEST_WIN() {
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
		answer >= CommandSerial::SELECT)
		if (!CUI.ReferClickable())
			return;

	/*�}�E�X�ŃR�}���h���󂯂Ȃ������ꍇ�̓L�[�{�[�h����R�}���h���󂯎��*/
	if (answer == CommandSerial::NONE_COMMAND)
		answer = CT.GetCommand();

	/*�R�}���h���Ȃ���΂��̊֐����I������*/
	if (answer == CommandSerial::NONE_COMMAND)
		return;

	/*�D�𑀍�*/
	auto ship = alliesFleet.begin();
	ship->ControlThisShip(answer);

	CUI.SetClickTime();//�N���b�N�������Ԃ��L�^����
	
	/*select�ȑO�̃R�}���h�̏���*/
	if (answer < CommandSerial::SELECT) {
		switch (answer) {
		case CommandSerial::SHOOT:TestShoot(); break;/*�ˌ�*/
		case CommandSerial::MENU:CUI.LetMeSeeMenu();break;/*���j���[��ԕύX*/
		case CommandSerial::TEST_VIEW_ON:TEST_SHOW_ON = !TEST_SHOW_ON; break;	/*�e�X�g�r���[*/
		case CommandSerial::EXIT:GameOver = true; break;	/*�Q�[���I��*/
		}
	}

	else {
	
		ship->fireDataFigureUp.SetNumber(answer -
			CommandSerial::SELECT - CommandSerial::SELECT_RANGE);//�^�[�Q�b�g��ݒu����
		ship->fireDataFigureUp.LockOn_Switch();//���b�N��Ԃ�ύX
		ship->ResetReviseData();//�C���f�[�^�����Z�b�g
		CUI.SetShootMenu(ship->fireDataFigureUp.ReferLockOn());//�t�h��ύX
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
		ship->Move();//�F�R�̑D���ړ�����
	}
	if(!enemyFleet.empty())//���X�g�m�F
	for (auto ship = enemyFleet.begin();
		ship != enemyFleet.end();
		ship++) {
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
void IngameDataManagement::Inif() {
	PL.InifForGame();//�摜���[�_�[������
	UI.InifUI(&PL);//UI������
	ET.InifEffectTemplate(&PL);//�G�t�F�N�g�e���v���[�g������
	SL.Inif();//�������[�_�[������
	CT.Inif(&SL);//�L�[�{�[�h�R���g���[���[������
	CUI.IngameInif(&PL,&SL);//�}�E�X�R���g���[���[������
	TEST();
	CUI.InifShipList(&enemyFleet,false);
	CUI.InifShipList(&alliesFleet, true);
	CUI.SetNormalStatus();/*�����̓e�X�g�o�[�W����*/
}

/*�g�������������������*/
void IngameDataManagement::Free() {
	PL.FREE_FOR_GAME();//���I������������A���������̃f�[�^���폜
	SL.FreeAll();//���I������������A���������̃f�[�^���폜
	CUI.Free();//�t�h�����̓��I���������������
	DestroyShips();//�D�����p�������I���������������
}

/*�������Ԓ��������̂�����*/
void IngameDataManagement::DeleteUseless() {
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
			if (abs(ship->ReferSpeedOnZ()) > 0.05 && rand() % 3 == 0
				&& rand() % 100 > ship->ReferSpeedOnZ() * 10) {//���̑��x������΁A�m���Ő�������
				//for(int i = 0; i < ship->ReferBubbleCount(); i++)
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
					&& rand() % 4 == 0) {//�m���Ő�������
					//for(int i = 0; i < ship->ReferSmokeCount(); i++)
					for (int i = 0; i < 2; i++)
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
	Effect effect(false, 500, radian, 0, 0, 0, Coord.x, Coord.z,
		PL.ReferEffectList(TypeOfEffect::EXPLOSION), true,
		0.02 * (double)(rand()%4), 1.05);

	explosionList.push_back(effect);//�������ꂽ���̂����X�g�̌��ɒǉ�����
}

void IngameDataManagement::NewRipple(double coordX, double coordZ) {
	Effect ripple(false, 1000, 0, 0, 0, 0, coordX, coordZ,
		PL.ReferBubbleHandle(), true, 0.3, 1.09);
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

void IngameDataManagement::TestShoot() {
	FiringData FD;
	//�Q�[�����ł́A�G���ˌ����鎞�ɑS��true�ɂ���
	FD.isThisMainWeapon = true;
	for (int i = 0; i < 8; i++) {
		FD.selected[i] = true;//�����I��
	}
	
	auto ship = alliesFleet.begin();
	InputNewAmmo(&*ship, FD);//���X�g�ɐV�����e��ǉ�����
}

void IngameDataManagement::InputNewAmmo(ShipMain *SM, FiringData FD) {
	int weaponAmount = SM->ReferWeaponCount(FD.isThisMainWeapon);//�I�񂾕�����m�F����
	for (int i = 0; i < weaponAmount; i++) {
		if (FD.selected[i]) {
			if (SM->IsThisOneUsable(i, FD.isThisMainWeapon)) {//�ˌ����\�ł����
				shellList.push_back(SM->Shoot(i,FD.isThisMainWeapon));//�V�����e��ǉ�����
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
	}
}

bool IngameDataManagement::PointsToCollisionbox(ShipMain *ship1, ShipMain *ship2) {
	for (int i = 0; i < ship1->ReferCollisionPointAmount(); i++) {
		if (!ship2->ReferAlive())//�D�ɂ̏�Ԃ��m�F
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
		if (shell->ReferSerialNumber() != ship->ReferSerialNumber()) {
			//�e�ƑD�̂����蔻����s��
			if (crash3DtoPoint(ship->ReferCoord(),
				shell->ReferCoord(),ship->ReferShipCrashSize(),
				ship->ReferRadianOnZ()))
			if(ship->ReferAlive()){
				//����������
				ship->SufferDamage((int)shell->ReferDamage());//�_���[�W��^����
				Coordinate2D<double> C2D = { shell->ReferCoordX(),
				shell->ReferCoordZ() };
				NewExplosion(C2D);//���������Ƃ���ɔ����G�t�F�N�g�𐶐�
				shell->Unusable();//�e���g���Ȃ��Ȃ�
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
			if (!ship->ReferAlive()) {
				sinkingShip.push_back(*ship);//����ł��郊�X�g�ɒǉ�����
			}
		}
	}
}