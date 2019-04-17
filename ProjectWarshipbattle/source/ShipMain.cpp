#include "ShipMain.h"

ShipMain::~ShipMain()
{
}

void ShipMain::InifThisShip(int *ShipHandle, int *SShadowH ,int ShipNum,
	EffectTemplate ET, SoundLoader *SL) {
	SetPictureHandle(ShipHandle);
	SetShadowHandle(SShadowH);
	//GetDataFromShipdata(ShipNum);
	MemorySecure();
	LoadSound(SL);
	FindThosePoint();//�����蔻��p�|�C���g���擾����

	bubblePointCount = AUTO_SHIP_DATA::AROUND_POINT;
	for (int i = 0; i < AUTO_SHIP_DATA::AROUND_POINT; i++) {
		bubbleStartPoint[i] = ET.CopyFromCreateBubble();
		bubbleStartPoint[i].InifCoordinate(aroundPointPos[i].x,
			aroundPointPos[i].z, true, 3400, 20, true,
			0.15, 1.001);
	}//�A�����|�C���g��ݒu

	for (int i = 0; i < 2; i++) {
		smokeStartPoint[i] = ET.CopyFromCreateSmoke();
		smokeStartPoint[i].InifCoordinate(4, 0, true, 3000, 20, true,
			0.06, 1.005);
	}
	smokeStartPoint[0].InifCoordinate(14, 0, true, 2500, 20, true,
		0.06, 1.005);
	smokeStartPoint[1].InifCoordinate(-12, 0, true, 2500, 20, true,
		0.06, 1.005);
}

void ShipMain::FindThosePoint() {
	using namespace AUTO_SHIP_DATA;

	int sx, sz;
	GetGraphSize(*ReferGraphHandle(), &sx, &sz);//�摜�̑傫�������
	int hTemp = MakeScreen(sx, sz, TRUE);//�󂫂̃O���t�B�b�N���쐬����
	SetDrawScreen(hTemp);//�`�����w��
	ClearDrawScreen();//��ʃN���A
	DrawGraph(0, 0, *ReferGraphHandle(), TRUE);//�D��`��
	ScreenFlip();
	unsigned int Cr = GetPixel(1, 1);//���ߐF
	bool flg = false;
	int num = 0;//�z��ԍ�


	//������
	for (int i = 0; i < AROUND_POINT; i++) {
		aroundPointPos[i].x = aroundPointPos[i].z = 0;
	}
	for (int xx = (int)sx / (AROUND_POINT / 2);
		xx < (int)sx - (int)sx / 
		(AROUND_POINT / 2);
		xx += (int)sx / (AROUND_POINT / 2)) {
		// �ォ�牺��1�h�b�g�����ׂ�
		for (int yy = 0; yy < (int)sz; yy++) {
			if (GetPixel(xx, yy) != Cr) {	// ���ߐF�ł͂Ȃ��Ȃ����Ƃ�
				aroundPointPos[num].x = xx * ReferMultiple() -
					sx * ReferMultiple() * .5;			// ���W�̓o�^
				aroundPointPos[num].z = yy * ReferMultiple() -
					sz * ReferMultiple() * .5;
				if (num++ >= AROUND_POINT - 1) flg = true;	// �z����I�[�o�[�������߂�t���O
				break;
			}
		}
		if (flg) break;	// �z����I�[�o�[�������߂�
		// ��������1�h�b�g�����ׂ�
		for (int yy = (int)sz - 1; yy > 0; yy--) {
			if (GetPixel(xx, yy) != Cr) {	// ���ߐF�ł͂Ȃ��Ȃ����Ƃ�
				aroundPointPos[num].x = xx * ReferMultiple() -
					sx * ReferMultiple() * .5;			// ���W�̓o�^
				aroundPointPos[num].z = yy * ReferMultiple() -
					sz * ReferMultiple() * .5;
				if (num++ >= AROUND_POINT - 1) flg = true;	// �z����I�[�o�[�������߂�t���O
				break;
			}
		}
		if (flg) break;	// �z����I�[�o�[�������߂�
	}
	pointCount = num;
}

/*�o�̓p�[�Z���e�[�W�ύX*/
void ShipMain::ChangeAccPercentage(bool up) {
	if (up&&currentAccPercentage < 1.0f) {
		currentAccPercentage += 0.25f;
		mainEngine.SetOutPutPercentage(currentAccPercentage);
	}
	if (!up&&currentAccPercentage > -0.25f) {
		currentAccPercentage -= 0.25f;
		mainEngine.SetOutPutPercentage(currentAccPercentage);
	}
}

void ShipMain::CalSpeed() {
	/*�������Z�ɕς��܂���*/
	mainEngine.Update();
	SetOutput(mainEngine.ReferOutput());
}

/*�i�s�����ύX*/
void ShipMain::ChangeDirect(bool right) {
	if (right) {
		currentRadian += radianChangePerFrame;
	}
	else {
		currentRadian -= radianChangePerFrame;
	}
	if (currentRadian > maxRadian)
		currentRadian = maxRadian;
	if (currentRadian < -maxRadian)
		currentRadian = -maxRadian;
}

/*�ǂ������I�ɐ^���ɖ߂邩*/
void ShipMain::Alignment() {

	if (returnToCenter == true) {
		if (currentRadian > MathAndPhysics::PI / 10000)
			currentRadian -= MathAndPhysics::PI / 10000;
		if (currentRadian < -MathAndPhysics::PI / 10000)
			currentRadian += MathAndPhysics::PI / 10000;
		if ((currentRadian < MathAndPhysics::PI / 10000 
			&& currentRadian>0) ||
			(currentRadian > -MathAndPhysics::PI / 10000 
				&& currentRadian < 0)) {
			currentRadian = 0;
		}
		if (currentRadian == 0)
			returnToCenter = false;
	}
	SetRadianChangePerFrame(currentRadian);
}

/*�����̏����̓R�}���h�Ɋւ����̂����ł�*/
void ShipMain::ControlThisShip(int Command) {;
	switch (Command) {
	case CommandSerial::NONE_COMMAND:break;
	case CommandSerial::INCREASE_OUTPUT:ChangeAccPercentage(true); break;
	case CommandSerial::DECREASE_OUTPUT:ChangeAccPercentage(false); break;
	case CommandSerial::TURN_RIGHT:ChangeDirect(true); break;
	case CommandSerial::TURN_LEFT:ChangeDirect(false); break;
	case CommandSerial::TURN_RETURN:ReturnDirectChange(); break;
	case CommandSerial::TURRET_TURN_RIGHT:TurnMainWeapon(true); break;
	case CommandSerial::TURRET_TURN_LEFT:TurnMainWeapon(false); break;
	case CommandSerial::TURRET_PULLUP:PullMainWeapon(true); break;
	case CommandSerial::TURRET_PULLDOWN:PullMainWeapon(false); break;
	}
}

void ShipMain::Update() {
	CalSpeed();
	Alignment();
	CalThePoint();
}

void ShipMain::TEST() {
	/*�����ɓ��ꂽ���͍̂Ō�S��shipdata�ɓ�������*/
	SetMass(30000);

	/*�G���W����Ԃ̏�����*/
	mainEngine.Inif(30, 0.75);
	SetMaxOutput(mainEngine.ReferMaxOutput());
	SetMaxSpeedOnZ(0.8);

	/*�ړ��֘A*/
	SetSpeed(0);
	maxRadian = MathAndPhysics::PI / 10;
	radianChangePerFrame = MathAndPhysics::PI / 1800;
	currentRadian = 0;

	serialNumber = 1;
	shipCrashR = 200;

	/*�G�t�F�N�g�֘A*/
	smokePointCount = 2;

	/*�����֘A*/
}

/*�f�o�b�O�p�e�X�g�֐�*/
void ShipMain::TestDraw(double x, double z) {
	for (int i = 0; i < bubblePointCount; i++) {
		bubbleStartPoint[i].TestDraw(ReferRadianOnZ(),ReferCoordX(),
			ReferCoordZ(),x,z);
	}
	for (int i = 0; i < 2; i++) {
		smokeStartPoint[i].TestDraw(ReferRadianOnZ(), ReferCoordX(),
			ReferCoordZ(),x,z);
	}
}

void ShipMain::MemorySecure() {
	MainParts = new ShipCrashParts [10];
	SubParts = new ShipCrashParts[10];

	MainWeapon = new Weapon[10];
	SubWeapon = new Weapon[10];

	bubbleStartPoint = new EffectPoint[AUTO_SHIP_DATA::AROUND_POINT];
	smokeStartPoint = new EffectPoint[2];
}

/*�폜�O�ɕK�����̊֐����Ăяo���āI*/
void ShipMain::DestroyMemory() {
	delete[] MainParts;
	delete[] SubParts;
	delete[] MainWeapon;
	delete[] SubWeapon;
	delete[] bubbleStartPoint;
	delete[] smokeStartPoint;
}

//�G�t�F�N�g����
Effect ShipMain::NewBubble(int num) {
	double newRadian = ReferRadianOnZ() - MathAndPhysics::PI;

	return bubbleStartPoint[num].NewEffect(newRadian,
		ReferSpeedOnZ(),
		ReferCoordX(), ReferCoordZ());
}

Effect ShipMain::NewSmoke(int num) {
	double newRadian = ReferRadianOnZ() - MathAndPhysics::PI;

	//��̂̕������ς��Ȃ����A���������ς��悤�ɂ���
	newRadian += (double)((1 - rand() % 3) / 180.0f) * MathAndPhysics::PI;

	//���������G�t�F�N�g�����������ĕԂ�
	return smokeStartPoint[num].NewEffect(newRadian, ReferSpeedOnZ(),
		ReferCoordX(), ReferCoordZ());
}

/*�����֘A*/
void ShipMain::LoadSound(SoundLoader *SL) {
	soundEngine = SL->referShipSoundEngine();
	soundMoving = SL->referShipSoundMoving();
	soundSlow = SL->referShipSoundSlow();
	soundTurning = SL->referShipSoundTuring();

	soundHordHigh = SL->referShipHordLargeHigh();
	soundHordLow = SL->referShipHordLargeLow();
}

void ShipMain::CheckAndPlaySound() {
	/*�G���W����*/
	if (currentAccPercentage != 0 && CheckSoundMem(*soundEngine) == 0) {
		PlaySoundMem(*soundEngine, DX_PLAYTYPE_LOOP, TRUE);
	}
	if (currentAccPercentage == 0 && CheckSoundMem(*soundEngine) != 0) {
		StopSoundMem(*soundEngine);
	}
	/*����*/
	if (ReferSpeedOnZ() > 0 && ReferSpeedOnZ() < 0.2&&
		CheckSoundMem(*soundSlow) == 0) {
		PlaySoundMem(*soundSlow,DX_PLAYTYPE_LOOP,TRUE);
	}
	if ((ReferSpeedOnZ() < 0 || ReferSpeedOnZ() >= 0.2)&&
		CheckSoundMem(*soundSlow) != 0) {
		StopSoundMem(*soundSlow);
	}

	if (ReferSpeedOnZ() >= 0.2 && CheckSoundMem(*soundMoving) == 0) {
		PlaySoundMem(*soundMoving, DX_PLAYTYPE_LOOP, TRUE);
	}
	if (ReferSpeedOnZ() < 0.2 && CheckSoundMem(*soundMoving) != 0) {
		StopSoundMem(*soundMoving);
	}

	/*�]����*/
	if (abs(currentRadian) > MathAndPhysics::PI / 30 &&
		CheckSoundMem(*soundTurning) == 0) {
		PlaySoundMem(*soundTurning, DX_PLAYTYPE_LOOP, TRUE);
	}
	if (abs(currentRadian) < MathAndPhysics::PI / 30 &&
		CheckSoundMem(*soundTurning) != 0) {
		StopSoundMem(*soundTurning);
	}
}

/*����̎g�����Ԃ��m�F*/
bool ShipMain::IsThisOneUsable(int Num, bool Main) {
	if (Main)
		return MainWeapon[Num].ReferShootable();
	else
		return SubWeapon[Num].ReferShootable();
}

Ammo ShipMain::Shoot(int Num, bool Main) {
	if (Main)
		return MainWeapon[Num].Shoot(ReferCoordX(),ReferCoordZ(),ReferRadianOnZ());
	else
		return SubWeapon[Num].Shoot(ReferCoordX(),ReferCoordZ(), ReferRadianOnZ());
}

void ShipMain::SetWeaponTest(PictureLoader *PL) {
	MainWeaponCount = 8;
	
	for (int i = 0; i < MainWeaponCount; i++) {
		Weapon Weapon(20.0, 8.0 - i * 2, 10.0, 10.0,
			0.0, 0.2, 100,
			50, 12, PL->ReferAmmoHandle(0), 17, 20,serialNumber);
		MainWeapon[i] = Weapon;
		MainWeapon[i].SetCoolDownTime(3200);//3200
	}
	fireControllerMain.SetSpeed(MainWeapon[0].ReferInitialSpeed());
	//���x��ǂݍ��݁A����������
	fireControllerMain.InifDistance(MainWeapon[0].ReferMaxRadianOnY(), 0);
}

void ShipMain::ShowMePointOfImpact(Camera camera) {
	DrawMainPoint(camera);
}

//���C������̊p�x��ύX
//return��true�̎��͐����͈͂Ɏ���A�Ȃ����false��Ԃ�
bool ShipMain::TurnMainWeapon(bool right) {
	for (int i = 0; i < MainWeaponCount; i++) {
		MainWeapon[i].Turn(right);
	}
	return false;
}

bool ShipMain::PullMainWeapon(bool up) {
	bool end;
	for (int i = 0; i < MainWeaponCount; i++) {
		end = MainWeapon[i].Pull(up);
	}
	return end;
}

void ShipMain::CalThePoint() {
	CalMainPoint();
}

void ShipMain::CalMainPoint() {
	/*�ˌ��f�[�^�͌v�Z�����Ɏg���邽�߁A�{�Ԃ̌v�Z�͍Ō�ɂ���*/
	fireControllerMain.SetCoord(MainWeapon[0].ReferRealCoord(ReferCoord(),
		ReferRadianOnZ()));//�D�̍��W��ݒ�
	RadianNeededIn3D temp = MainWeapon[0].ReferRadian();
	temp.z += ReferRadianOnZ();//�{�Ԃ̊p�x�ɐݒ肷��
	fireControllerMain.SetRadian(temp);//���C���̊p�x��ݒ�
	fireControllerMain.CalTheAnswer();//�ڕW���W���v�Z����
}


void ShipMain::DrawMainPoint(Camera camera) {
	Coordinate<double> point = fireControllerMain.ReferAnswer();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 170);//�����x��������
	DrawCircleAA((float)(point.x - camera.ReferRealCameraX()),
		(float)(point.z-camera.ReferRealCameraZ()),
		15,16,GetColor(255,0,0),0,0.5);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);//�����x��������
}

/*�퓬����*/
void ShipMain::SufferDamage(int damage) {
	hitPoint -= damage;
	if (hitPoint <= 0)
		Killed();
}

/*���b�N�Ǘ�*/
void ShipMain::TestLock(ShipMain *ship) {
	CalDistance(ship);//�������v�Z����

	double x1, x2, z1, z2;
	x1 = ship->ReferCoordX();
	x2 = ReferCoordX();
	z1 = ship->ReferCoordZ();
	z2 = ReferCoordZ();

	//�t�O�p�֐��𗘗p���ă��W�A�����v�Z����
	targetRadianOnZforMain = atan((z2 - z1) / (x2 - x1));

	if (x1 > x2 && z1 < z2) {//�G�͑��ی��ɂ���
		targetRadianOnZforMain = targetRadianOnZforMain;
	}
	else if (x1 > x2 && z1 > z2) {//�G�͑��ی��ɂ���
		targetRadianOnZforMain = -targetRadianOnZforMain;
	}
	else if (x1 < x2 && z1 > z2) {//�G�͑�O�ی��ɂ���
		targetRadianOnZforMain = targetRadianOnZforMain + MathAndPhysics::PI;
	}
	else {//�G�͑�l�ی��ɂ���
		targetRadianOnZforMain = MathAndPhysics::PI - targetRadianOnZforMain;
	}

	//targetRadianOnZforMain = asin((z2 - z1) / sqrt((pow(z2 - z1, 2) + pow(x2 - x1, 2))));

	/*�����p�x�̌v�Z*/
	targetRadianForMain = fireControllerMain.CalDistanceAndTellMeRadianOnY(distance);

	/*���C������̒���*/
	/*����*/
	if (MainWeapon[0].ReferRadianOnY() - targetRadianForMain > 0.03) {
		PullMainWeapon(false);
	}
	else if (MainWeapon->ReferRadianOnY() - targetRadianForMain < 0.03) {
		PullMainWeapon(true);
	}
	/*����*/
	if (MainWeapon[0].ReferRadianOnZ() + ReferRadianOnZ() -
		targetRadianOnZforMain > 0.03) {
		TurnMainWeapon(false);
	}
	else if (MainWeapon[0].ReferRadianOnZ() + ReferRadianOnZ() -
		targetRadianOnZforMain < 0.03) {
		TurnMainWeapon(true);
	}
}

void ShipMain::CalDistance(ShipMain *ship) {
	distance = Distance2D(ship->ReferCoord(), ReferCoord());
}