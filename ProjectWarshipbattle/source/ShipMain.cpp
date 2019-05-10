#include "ShipMain.h"

ShipMain::~ShipMain()
{
}

void ShipMain::InifThisShip(int *ShipHandle, int *SShadowH, int ShipNum,
	EffectTemplate ET, SoundLoader *SL) {
	SetPictureHandle(ShipHandle);
	SetShadowHandle(SShadowH);
	//GetDataFromShipdata(ShipNum);
	thisShipType = ShipNum;
	MemorySecure();//�������m��
	LoadSound(SL);//����ǂݍ���
	FindThosePoint();//�����蔻��p�|�C���g���擾����

	/*�����蔻��p�|�C���g�𗘗p���ĖA�����|�C���g�����*/
	bubblePointCount = AUTO_SHIP_DATA::AROUND_POINT;
	for (int i = 0; i < AUTO_SHIP_DATA::AROUND_POINT; i++) {
		bubbleStartPoint[i] = ET.CopyFromCreateBubble();
		bubbleStartPoint[i].InifCoordinate(aroundPointPos[i].x,
			aroundPointPos[i].z, true, 3400, 20, true,
			0.15, 1.001);
	}//�A�����|�C���g��ݒu

	for (int i = 0; i < 2; i++) {
		smokeStartPoint[i] = ET.CopyFromCreateSmoke();//�e���v���[�g����R�s�[����
		/*����������*/
		smokeStartPoint[i].InifCoordinate(4, 0, true, 3000, 20, true,
			0.06, 1.005);
	}
	smokeStartPoint[0].InifCoordinate(14, 0, true, 2500, 20, true,
		0.06, 1.005);
	smokeStartPoint[1].InifCoordinate(-12, 0, true, 2500, 20, true,
		0.06, 1.005);
}

/*�t�@�C���ǂݍ��݂����s������false��߂�*/
bool ShipMain::InifThisShip(PictureLoader *PL, EffectTemplate ET,
	SoundLoader *SL, int num,int SN) {

	int count = 0;

	char readData[50][50];
	char trash[50];
	double doubleData[50];

	serialNumber = SN;

	/*�t�@�C������*/
	FILE *filePointer;//�t�@�C���|�C���^�[
	char fileName[100];
	sprintf_s(fileName, 100, ".//Data//ShipNumList//%d.csv", num);

	if ((fopen_s(&filePointer, fileName, "r")) != 0) {
		return false;
	}

	/*�ǂݍ���*/
	while (1) {
		fscanf_s(filePointer,"%[^,]", &readData[count], 20);//������ǂ�
		doubleData[count] = atof(readData[count]);
		fscanf_s(filePointer,"%*[,]%s", &trash, 20);

		count++;
		if (feof(filePointer))/*�ǂݍ��ݒ��~*/
			break;
	}

	fclose(filePointer);//�t�@�C�������

	SetPictureHandle(PL->ReferShipHandle(num));
	SetShadowHandle(PL->ReferShipShadowHandle(num));
	SetLength(PL->ReferShipSizeX());
	SetWidth(PL->ReferShipSizeZ());
	thisShipType = num;
	MemorySecure();//�������m��
	LoadSound(SL);//����ǂݍ���
	FindThosePoint();//�����蔻��p�|�C���g���擾����

	/*�����蔻��p�|�C���g�𗘗p���ĖA�����|�C���g�����*/
	bubblePointCount = AUTO_SHIP_DATA::AROUND_POINT;
	for (int i = 0; i < AUTO_SHIP_DATA::AROUND_POINT; i++) {
		bubbleStartPoint[i] = ET.CopyFromCreateBubble();
		bubbleStartPoint[i].InifCoordinate(aroundPointPos[i].x,
			aroundPointPos[i].z, true, 3400, 20, true,
			0.15, 1.001);
	}//�A�����|�C���g��ݒu

	int pointer = 0;//�f�[�^��ǂݍ��ޗp�|�C���^�[
					
	SetMultiple(doubleData[pointer]);
	pointer++;
	//�g�o��ݒu����
	maxHitPoint = hitPoint = (int)doubleData[pointer];
	pointer++;

	//�X���[�N�����_��ݒu����
	smokePointCount  = (int)doubleData[pointer];
	pointer++;

	int smokeTime;//�X���[�N�̌p�����Ԃ�ݒu
	smokeTime = (int)doubleData[pointer];
	pointer++;

	for (int i = 0; i < smokePointCount; i++) {
		smokeStartPoint[i] = ET.CopyFromCreateSmoke();//�e���v���[�g����R�s�[����
		/*����������*/
		smokeStartPoint[i].InifCoordinate(doubleData[pointer],
			doubleData[pointer + 1], true, smokeTime, 20, true,
			0.06, 1.005);
		pointer += 2;
	}

	/*����:�ݒu������Ƀ|�C���^�[�͂������̍s�Ɏw���Ă��邩��{�{�����Ȃ���*/
	/*�����蔻��p�{�b�N�X��ݒu����*/
	shipMainCrash = { doubleData[pointer],doubleData[pointer + 1], 
		doubleData[pointer + 2]};
	pointer += 3;

	/*�d�͂�ݒu����*/
	SetMass(doubleData[pointer]);
	pointer++;
	/*�G���W���̏o�͂�ݒu����*/
	mainEngine.Inif(doubleData[pointer], doubleData[pointer + 1]);
	pointer += 2;
	SetMaxOutput(mainEngine.ReferMaxOutput());//�ő�o�͂�AllMovableObject�ɐݒ肷��
	/*�ő呬�x��ݒu����*/
	SetMaxSpeedOnZ(doubleData[pointer]);
	pointer++;

	SetSpeed(0);//���̑��x���O�ɐݒ肷��
	maxRadian = MathAndPhysics::PI / 6;//�ő�p�x��18�x�ɐݒ肷��
	radianChangePerFrame = MathAndPhysics::PI / 900;//�ǐ��񑬓x��ݒu
	currentRadian = 0;//�ǂ����_�ɐݒ肷��

	/*����ʗp�X�e�[�^�X���L�^*/
	int weaponCD = (int)doubleData[pointer];
	pointer++;
	int weaponDamage = (int)doubleData[pointer];
	pointer++;
	double weaponHigh = doubleData[pointer];
	pointer++;
	double weaponInitialSpeed = doubleData[pointer];
	pointer++;

	MainWeaponCount = (int)doubleData[pointer];
	pointer++;

	for (int i = 0; i < MainWeaponCount; i++) {
		Weapon Weapon(doubleData[pointer], doubleData[pointer + 1],
			10.0, 10.0,//�����Ɖ��̒���-�܂��g���Ă��܂���
			0.0, 0.2, 100,//�����p�x�A�g�o�i�܂��g���Ă��܂���j
			50, 12, //���b�l�i�܂��g���Ă��܂���j
			PL->ReferAmmoHandle(0), weaponInitialSpeed, weaponHigh,
			serialNumber);
		MainWeapon[i] = Weapon;
		MainWeapon[i].SetCoolDownTime(weaponCD);//�ˌ��Ԋu��ݒ肷��
		MainWeapon[i].SetDamage(weaponDamage);
		pointer += 2;
	}
	/*����̃X�e�[�^�X���ˌ��R���g���[���ɂ�����*/
	fireControllerMain.SetSpeed(MainWeapon[0].ReferInitialSpeed());
	//���x��ǂݍ��݁A����������
	fireControllerMain.InifDistance(MainWeapon[0].ReferMaxRadianOnY(), 0);
	
	return true;
}

/*�t�@�C���������ꂽ�̂��߁A�������̑��x���啝�����Ȃ�܂���*/
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

	FILE *filePointer;//�t�@�C���|�C���^�[
	char fileName[100];
	sprintf_s(fileName, 100, ".//Data//Points//%d.txt", thisShipType);

	/*�����ۑ����ꂽ�f�[�^������ꍇ�͂��̂܂ܓǂݍ���*/
	if ((fopen_s(&filePointer, fileName, "r")) == 0) {
		int j;
		fscanf_s(filePointer, "%d\n", &j);
		for (int i = 0; i < j; i++) {
			fscanf_s(filePointer, "%lf %lf\n", &aroundPointPos[i].x, &aroundPointPos[i].z);
			/*�����̓G���[�������K�v���Ǝv���܂�*/
		}
		
		pointCount = AROUND_POINT;
		fclose(filePointer);
		return;//�ǂݍ��񂾂�֐����I��
	}

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

	/*�����t�@�C�������Ȃ������ꍇ�͐V���Ƀt�@�C��������āA�f�[�^��ۑ�����*/
	fopen_s(&filePointer, fileName, "w");
	fprintf(filePointer, "%d\n", num);
	for (int i = 0; i < AROUND_POINT; i++) {
		fprintf(filePointer, "%lf %lf\n", aroundPointPos[i].x, aroundPointPos[i].z);
		//���W���L�^����
	}
	fclose(filePointer);//�t�@�C�������

	pointCount = num;//�����L�^����
}

/*�o�̓p�[�Z���e�[�W�ύX*/
void ShipMain::SetEngineOutPutRate(bool up) {
	if (up&&currentEngineOutPutRate < 1.0f) {//�͈͓��ł����
		currentEngineOutPutRate += 0.25f;	//��������
	}
	if (!up&&currentEngineOutPutRate > -0.25f) {//�͈͓��ł����
		currentEngineOutPutRate -= 0.25f; //��������
	}
	mainEngine.SetOutPutPercentage(currentEngineOutPutRate);//���̏o�͂�^����
}

void ShipMain::CalSpeed() {
	/*�������Z�ɕς��܂���*/
	mainEngine.Update();//�G���W���̃X�e�[�^�X���X�V
	SetOutput(mainEngine.ReferOutput());//�X�V���ꂽ�o�͂�ݒ肷��
}

/*�i�s�����ύX*/
void ShipMain::ChangeDirect(bool right) {
	if (right) {
		currentRadian += radianChangePerFrame;//�E����
	}
	else {
		currentRadian -= radianChangePerFrame;//������
	}
	if (currentRadian > maxRadian)//�E�͈̔͌���
		currentRadian = maxRadian;
	if (currentRadian < -maxRadian)//���͈̔͌���
		currentRadian = -maxRadian;
}

/*�ǂ������I�ɐ^���ɖ߂邩*/
void ShipMain::Alignment() {

	if (returnToCenter == true) {
		//�ǂ��E�ɂ��鎞
		if (currentRadian > MathAndPhysics::PI / 2000)
			currentRadian -= MathAndPhysics::PI / 2000;
		//�ǂ����ɂ��鎞
		if (currentRadian < -MathAndPhysics::PI / 2000)
			currentRadian += MathAndPhysics::PI / 2000;
		/*���S�ɋ߂��Ȃ�ƒ��S�ɒu���悤�ɂ���*/
		if ((currentRadian < MathAndPhysics::PI / 2000
			&& currentRadian>0) ||
			(currentRadian > -MathAndPhysics::PI / 2000 
				&& currentRadian < 0)) {
			currentRadian = 0;
		}
		/*�ǂ����S�ɂ��鎞�Ɏ����߂��Ԃ�����*/
		if (currentRadian == 0)
			returnToCenter = false;
	}
	SetRadianChangePerFrame(currentRadian);//���̏�Ԃ��X�V����
}

/*�����̏����̓R�}���h�Ɋւ����̂����ł�*/
void ShipMain::ControlThisShip(int Command) {;
	switch (Command) {
	case CommandSerial::NONE_COMMAND:break;
	case CommandSerial::INCREASE_OUTPUT:SetEngineOutPutRate(true); break;
	case CommandSerial::DECREASE_OUTPUT:SetEngineOutPutRate(false); break;
	case CommandSerial::TURN_RIGHT:ChangeDirect(true); break;
	case CommandSerial::TURN_LEFT:ChangeDirect(false); break;
	case CommandSerial::TURN_RETURN:ReturnDirectChange(); break;
	case CommandSerial::TURRET_TURN_RIGHT:TurnMainWeapon(true); break;
	case CommandSerial::TURRET_TURN_LEFT:TurnMainWeapon(false); break;
	case CommandSerial::TURRET_PULLUP:PullMainWeapon(true); break;
	case CommandSerial::TURRET_PULLDOWN:PullMainWeapon(false); break;
	case CommandSerial::FORECAST_PLUS:ChangeForecastSecond(true); break;
	case CommandSerial::FORECAST_MINUS:ChangeForecastSecond(false); break;
	case CommandSerial::REVISE_RADIAN_ON_Z_PLUS:ChangeReviseRadianOnY(true); break;
	case CommandSerial::REVISE_RADIAN_ON_Z_MINUS:ChangeReviseRadianOnY(false); break;
	}
}

/*�t���[�����ƂɍX�V����*/
void ShipMain::Update() {
	CalSpeed();//�o�͂��v�Z����
	Alignment();//�p�x���v�Z����
	CalThePoint();//�C�e�����n�_���v�Z����
}

void ShipMain::TEST() {
	/*�����ɓ��ꂽ���͍̂Ō�S��shipdata�ɓ�������*/
	SetMass(30000);

	/*�G���W����Ԃ̏�����*/
	mainEngine.Inif(30, 0.75);
	SetMaxOutput(mainEngine.ReferMaxOutput());//�ő�o�͂�AllMovableObject�ɐݒ肷��
	SetMaxSpeedOnZ(0.8);//�ő呬�x��ݒ肷��

	/*�ړ��֘A*/
	SetSpeed(0);//���̑��x���O�ɐݒ肷��
	maxRadian = MathAndPhysics::PI / 6;//�ő�p�x��18�x�ɐݒ肷��
	radianChangePerFrame = MathAndPhysics::PI / 900;//�ǐ��񑬓x��ݒu
	currentRadian = 0;//�ǂ����_�ɐݒ肷��

	serialNumber = 1;//�ԍ�
	shipCrashR = 200;

	/*�G�t�F�N�g�֘A*/
	smokePointCount = 2;

	/*�����֘A*/
}

/*�f�o�b�O�p�e�X�g�֐�*/
void ShipMain::TestDraw(double x, double z) {
	//�����|�C���g��`�悷��
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
	MainParts = new ShipCrashParts[10];
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
	/*�D�Ɛ����΂̕����Ɍ���*/
	double newRadian = ReferRadianOnZ() - MathAndPhysics::PI;

	//�o�u���|�C���g����A�𐶐�����
	return bubbleStartPoint[num].NewEffect(newRadian,
		ReferSpeedOnZ(),
		ReferCoordX(), ReferCoordZ());
}

Effect ShipMain::NewSmoke(int num) {
	/*�D�Ɛ����΂̕����Ɍ���*/
	double newRadian = ReferRadianOnZ() - MathAndPhysics::PI;

	//��̂̕������ς��Ȃ����A���������ς��悤�ɂ���
	newRadian += (double)((1 - rand() % 3) / 180.0f) * MathAndPhysics::PI;

	//���������G�t�F�N�g�����������ĕԂ�
	return smokeStartPoint[num].NewEffect(newRadian, ReferSpeedOnZ(),
		ReferCoordX(), ReferCoordZ());
}

/*�����֘A*/
void ShipMain::LoadSound(SoundLoader *SL) {
	/*�e�����̉������[�h����*/
	soundEngine = SL->referShipSoundEngine();
	soundMoving = SL->referShipSoundMoving();
	soundSlow = SL->referShipSoundSlow();
	soundTurning = SL->referShipSoundTuring();

	soundHordHigh = SL->referShipHordLargeHigh();
	soundHordLow = SL->referShipHordLargeLow();
}

void ShipMain::CheckAndPlaySound() {
	/*�G���W����*/
	if (currentEngineOutPutRate != 0 && CheckSoundMem(*soundEngine) == 0) {
		PlaySoundMem(*soundEngine, DX_PLAYTYPE_LOOP, TRUE);
	}
	if (currentEngineOutPutRate == 0 && CheckSoundMem(*soundEngine) != 0) {
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
	if (Main)//���C������
		return MainWeapon[Num].ReferShootable();
	else//�T�u����
		return SubWeapon[Num].ReferShootable();
}

Ammo ShipMain::Shoot(int Num, bool Main) {
	if (Main)//���C������
		return MainWeapon[Num].Shoot(ReferCoordX(),ReferCoordZ(),ReferRadianOnZ());
	else//�T�u����
		return SubWeapon[Num].Shoot(ReferCoordX(),ReferCoordZ(), ReferRadianOnZ());
}

void ShipMain::SetWeaponTest(PictureLoader *PL) {
	MainWeaponCount = 8;//����ݒ肷��
	
	for (int i = 0; i < MainWeaponCount; i++) {
		/*���������������*/
		Weapon Weapon(20.0, 8.0 - i * 2, 10.0, 10.0,
			0.0, 0.2, 100,
			50, 12, PL->ReferAmmoHandle(0), 17, 20,serialNumber);
		MainWeapon[i] = Weapon;
		MainWeapon[i].SetCoolDownTime(3200);//�ˌ��Ԋu��ݒ肷��
		MainWeapon[i].SetDamage(3);
	}
	/*����̃X�e�[�^�X���ˌ��R���g���[���ɂ�����*/
	fireControllerMain.SetSpeed(MainWeapon[0].ReferInitialSpeed());
	//���x��ǂݍ��݁A����������
	fireControllerMain.InifDistance(MainWeapon[0].ReferMaxRadianOnY(), 0);
}

void ShipMain::ShowMePointOfImpact(Camera camera) {
	DrawMainPoint(camera);//�����n�_��`��
}

//���C������̊p�x��ύX
//���͒l�F�E�֐��񂵂Ă��邩
//return��true�̎��͐����͈͂Ɏ���A�Ȃ����false��Ԃ�
bool ShipMain::TurnMainWeapon(bool right) {
	for (int i = 0; i < MainWeaponCount; i++) {
		MainWeapon[i].Turn(right);
	}
	return false;
}

//���C������̊p�x��ύX
//���͒l�F�オ���Ă��邩
//return��true�̎��͐����͈͂Ɏ���A�Ȃ����false��Ԃ�
bool ShipMain::PullMainWeapon(bool up) {
	bool end;
	for (int i = 0; i < MainWeaponCount; i++) {
		end = MainWeapon[i].Pull(up);
	}
	return end;
}

/*�����n�_���v�Z����*/
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

/*�C�e�����n�_�����ŕ\������*/
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
/*���b�N�I�����̑���*/
void ShipMain::ChangeForecastSecond(bool up) {
	if (up && forecastSeconds <= maxForecastSecond)
		forecastSeconds++;
	if (!up&&forecastSeconds >= -maxForecastSecond)
		forecastSeconds--;
}

/*���b�N�Ǘ�*/
void ShipMain::TestLock(ShipMain *ship, bool render) {

	if (forecastSeconds == 0)
		CalDistance(ship);//�������v�Z����
	else
		CalNextPos(ship);


	//�ڕW���W�A��
	double differenceOnY;
	double differenceOnZ;

	canIShoot = false;

	//�ڕW���W�A�����v�Z����
	differenceOnZ = CalRadianBetweenPoints(
		(forecastSeconds == 0) ? ship->ReferCoord2D_d() : nextPos,
		//�\�����g�����͗\���n�_���g���A�g��Ȃ����͓G�̍��W���g��
		ReferCoord2D_d(),
		MainWeapon[0].ReferRadianOnZ() + ReferRadianOnZ());

	/*�����p�x�̌v�Z�E�C��*/
	targetRadianForMain = fireControllerMain.CalDistanceAndTellMeRadianOnY(distance)
		+ fireDataFigureUp.Refercorrection().y;

	differenceOnY = MainWeapon[0].ReferRadianOnY() + reviseRadianOnZ
		- targetRadianForMain;

	/*���C������̒���*/
	/*����*/
	if (differenceOnY > 0.03) {
		PullMainWeapon(false);
	}
	else if (differenceOnY < 0.03) {
		PullMainWeapon(true);
	}

	/*���։񂷎�*/
	if (differenceOnZ > 0.03) {
		TurnMainWeapon(false);
	}
	/*�E�։񂷎�*/
	else if (differenceOnZ < 0.03) {
		TurnMainWeapon(true);
	}

	if (abs(differenceOnY) < 0.06 && abs(differenceOnZ) < 0.1)
		canIShoot = true;
}

void ShipMain::CalDistance(ShipMain *ship) {
	distance = Distance2D(ship->ReferCoord(), ReferCoord());
}

void ShipMain::CalNextPos(ShipMain *ship) {
	nextPos = ship->ReferCoord2D_d();
	NextPoint(&nextPos,ship->ReferRadianOnZ(), ship->ReferSpeedOnZ(),
		forecastSeconds*framesCountInaSecond);
	distance = Distance2D(nextPos, ReferCoord2D_d());
}

void ShipMain::ChangeReviseRadianOnY(bool up) {
	if (reviseRadianOnZ <= maxReviseRadianOnZ && !up)
		reviseRadianOnZ += MathAndPhysics::PI * MathAndPhysics::OneDegree * 0.25;
	if (reviseRadianOnZ >= -maxReviseRadianOnZ && up)
		reviseRadianOnZ -= MathAndPhysics::PI * MathAndPhysics::OneDegree * 0.25;
}