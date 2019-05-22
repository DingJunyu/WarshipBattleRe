#include "UserInterface.h"
using namespace DxLib;

UserInterface::~UserInterface()
{
}

void UserInterface::InifUI(PictureLoader *PL) {
	//�摜�ǂݍ���
	mat = PL->ReferMatHandle();
	shipCardMat = PL->ReferShipCardMatHandle();
	miniMapMat = PL->ReferMiniMapMatHandle();
	miniMapPaper = PL->ReferMiniMapPaperHandle();

	for (int i = UI_LIST::RADAR; i <= UI_LIST::CAMERA_ON_ENEMY; i++)
		handleList[i] = PL->ReferUIList(i);//�n���h�����R�s�[����

	Coordinate2D<float> coord;
	unsigned int color;
	//�d��������
	//���W���v�Z����
	coord.x = (float)(Screen::SCREEN_X)*(float)(LAMP_POSITION::RETURN_TO_MIDDLE_X);
	coord.z = (float)(Screen::SCREEN_Z)*(float)(LAMP_POSITION::RETURN_TO_MIDDLE_Z);
	//���W��d���ɑ��
	lamps[LAMP_LIST::RETURN_TO_MIDDLE].Inif(coord,
		(float)LAMP_POSITION::RETURN_TO_MIDDLE_R);
	//���b�N��Ԃ̎w��
	coord.x = (float)(Screen::SCREEN_X)*(float)(LAMP_POSITION::LOCKED_X);
	coord.z = (float)(Screen::SCREEN_Z)*(float)(LAMP_POSITION::LOCKED_Z);
	lamps[LAMP_LIST::LOCKED].Inif(coord,
		(float)LAMP_POSITION::LOCKED_R);
	//�I�t��Ԃ̐F��ݒ�
	color = GetColor(255, 0, 0);
	lamps[LAMP_LIST::RETURN_TO_MIDDLE].SetColor(false, color);
	lamps[LAMP_LIST::LOCKED].SetColor(false, color);
	//�I����Ԃ̐F��ݒ�
	color = GetColor(0, 255, 0);
	lamps[LAMP_LIST::RETURN_TO_MIDDLE].SetColor(true, color);
	lamps[LAMP_LIST::LOCKED].SetColor(true, color);
}

void UserInterface::DrawUI() {
	/*�ėp�ϐ�*/
	unsigned int Cr;//�F

	/*��*/
	DrawPictureByCenterOnX(UI_LIST::SHOW_DIRECT,
		USER_INTERFACE_POSITION::SHOW_DIRECT_X,
		USER_INTERFACE_POSITION::SHOW_DIRECT_Z,
		USER_INTERFACE_POSITION::SHOW_DIRECT_MULTI);

	//�`�����Ԃ𒍈ӁI
	/*���蕔��*/
	DrawExtendGraph(0, 0, Screen::SCREEN_X / 20, Screen::SCREEN_Z,
		*shipCardMat, FALSE);
	DrawExtendGraph(Screen::SCREEN_X * 19/20,0 ,Screen::SCREEN_X, 
		Screen::SCREEN_Z,
		*shipCardMat, FALSE);
	DrawExtendGraph((int)matStartX, Screen::SCREEN_Z - (int)matStartZ,
		Screen::SCREEN_X - 1, Screen::SCREEN_Z - 1, *mat, FALSE);

	/*�~�j�}�b�v*/
	DrawExtendGraph(Screen::SCREEN_X / 2 - (int)mapMatSize,
		Screen::SCREEN_Z - (int)(1.2 * mapMatSize),
		Screen::SCREEN_X / 2 + (int)mapMatSize ,
		Screen::SCREEN_Z, *miniMapMat, FALSE);

	DrawExtendGraph(Screen::SCREEN_X / 2 - (int)mapPaperSize,
		Screen::SCREEN_Z - (int)(1.2 * (int)mapPaperSize),
		Screen::SCREEN_X/2 + (int)mapPaperSize,
		Screen::SCREEN_Z - 5, *miniMapPaper, FALSE);

	/*�p�x�w��*/
	DrawPictureByLeftUp(UI_LIST::SHOW_RADIAN_ON_Y,
		USER_INTERFACE_POSITION::SHOW_RADIAN_ON_Y_X,
		USER_INTERFACE_POSITION::SHOW_RADIAN_ON_Y_Z,
		USER_INTERFACE_POSITION::SHOW_RADIAN_ON_Y_MULTI);

	/*����CD*/
	Cr = GetColor(0, 0, 0);
	DrawBox(
		(int)(USER_INTERFACE_POSITION::SHOOT_CD_MAIN_X * Screen::SCREEN_X),
		(int)(USER_INTERFACE_POSITION::SHOOT_CD_MAIN_Z * Screen::SCREEN_Z),
		(int)(USER_INTERFACE_POSITION::SHOOT_CD_MAIN_X * Screen::SCREEN_X) +
		(int)USER_INTERFACE_POSITION::SHOOT_CD_MAIN_SIZE_X,
		(int)(USER_INTERFACE_POSITION::SHOOT_CD_MAIN_Z * Screen::SCREEN_Z) +
		(int)USER_INTERFACE_POSITION::SHOOT_CD_MAIN_SIZE_Z,
		Cr,TRUE
	);

	/*���x�v*/
	DrawPictureByCenter(UI_LIST::SPEED_PAN,
		USER_INTERFACE_POSITION::SPEED_PAN_X,
		USER_INTERFACE_POSITION::SPEED_PAN_Z,
		USER_INTERFACE_POSITION::SPEED_PAN_MULTI);

	/*�o�͌v*/
	DrawPictureByCenter(UI_LIST::ENGINE_PAN,
		USER_INTERFACE_POSITION::ENGINE_PAN_X,
		USER_INTERFACE_POSITION::ENGINE_PAN_Z,
		USER_INTERFACE_POSITION::ENGINE_PAN_MULTI);

	/*�e�L�X�g�o�[*/
	/*�����w��*/
	DrawPictureByCenterOnX(UI_LIST::TEXT_BAR_DIRECT,
		USER_INTERFACE_POSITION::TEXT_BAR_DIRECT_X,
		USER_INTERFACE_POSITION::TEXT_BAR_DIRECT_Z,
		USER_INTERFACE_POSITION::TEXT_BAR_DIRECT_MULTI);


	/*���x�w��*/
	DrawPictureByLeftUp(UI_LIST::TEXT_BAR_SPEED,
		USER_INTERFACE_POSITION::TEXT_BAR_SPEED_X,
		USER_INTERFACE_POSITION::TEXT_BAR_SPEED_Z,
		USER_INTERFACE_POSITION::TEXT_BAR_SPEED_MULTI);

	/*�C������w��*/
	DrawPictureByLeftUp(UI_LIST::TEXT_BAR_TURRET,
		USER_INTERFACE_POSITION::TEXT_BAR_TURRET_X,
		USER_INTERFACE_POSITION::TEXT_BAR_TURRET_Z,
		USER_INTERFACE_POSITION::TEXT_BAR_TURRET_MULTI);
}

void UserInterface::DrawUINeedInput(ShipMain *ship, bool autoFire,
	bool changeCamera) {
	/*�ėp�ϐ�*/
	int graphSizeX, graphSizeZ;//�摜�T�C�Y
	int startX, startZ;//���W
	unsigned int Cr;//�F

	double radian;

	DrawPictureByCenterOnX(changeCamera ? UI_LIST::CAMERA_ON_ENEMY :
		UI_LIST::CAMERA_ON_ME,
		0.5, 0, 0.25
	);


	/*���b�N�g���ĂȂ����*/
	if (!ship->fireDataFigureUp.ReferLockOn()) {
		/*���[�_�[A*/
		DrawPictureByLeftUp(UI_LIST::RADAR,
			USER_INTERFACE_POSITION::RADAR_A_X,
			USER_INTERFACE_POSITION::RADAR_A_Z,
			USER_INTERFACE_POSITION::RADAR_A_MULTI);
		/*���[�_�[�̏�̖��*/
		radian = ship->ReferMainWeaponRadianOnZ() + ship->ReferRadianOnZ()
			- MathAndPhysics::PI / 4;
		DrawRotatePicture(UI_LIST::ARROW_RED,
			USER_INTERFACE_POSITION::ARROW_RED_A_X,
			USER_INTERFACE_POSITION::ARROW_RED_A_Z,
			USER_INTERFACE_POSITION::ARROW_RED_A_MULTI,
			radian);
	}
	/*���b�N���g���Ă�����*/
	else {
		/*knob*/
		DrawPictureByLeftUp(UI_LIST::FORECAST_BUTTON,
			USER_INTERFACE_POSITION::RADAR_A_X,
			USER_INTERFACE_POSITION::RADAR_A_Z,
			USER_INTERFACE_POSITION::RADAR_A_MULTI);
		/*�_*/
		radian = ship->ReferForecastSecond() * MathAndPhysics::PI * 
			MathAndPhysics::OneDegree * 6
			- MathAndPhysics::PI * 0.75;
		DrawRotatePicture(UI_LIST::FORECAST_POINT,
			USER_INTERFACE_POSITION::ARROW_RED_A_X,
			USER_INTERFACE_POSITION::ARROW_RED_A_Z,
			USER_INTERFACE_POSITION::ARROW_RED_A_MULTI,
			radian);
		lamps[LAMP_LIST::LOCKED].SetOnOrOff(ship->ReferCanIShoot());
		lamps[LAMP_LIST::LOCKED].Draw();
		/*�ˌ��\�}�[�N*/
		if (ship->ReferCanIShoot()) {
			DrawPictureByCenter(UI_LIST::UI_LOCK_ON,
				0.69, 0.69, 0.4);
		}
		/**/
		if (autoFire) {
			DrawPictureByLeftUp(UI_LIST::UI_AUTO_FIRE,
				0.84, 0.8, 0.4);
		}
	}

	/*�C���p�x�w��*/
	Cr = GetColor(255, 0, 0);//�F��ݒ肷��
	GetGraphSize(*handleList[UI_LIST::SHOW_RADIAN_ON_Y], &graphSizeX, &graphSizeZ);
	double Percentage;
	Percentage = 1 -
		ship->ReferMainWeaponRadianOnY() / ship->ReferMainMaxWeaponRadianOnY();
	Percentage *= graphSizeZ * USER_INTERFACE_POSITION::SHOW_RADIAN_ON_Y_MULTI;
	//�}�[�N�̈ʒu���v�Z����

	startX = (int)(Screen::SCREEN_X * USER_INTERFACE_POSITION::SHOW_RADIAN_ON_Y_X);
	startZ = (int)(Screen::SCREEN_Z * USER_INTERFACE_POSITION::SHOW_RADIAN_ON_Y_Z)
		+ (int)Percentage;
	if (startZ >= Screen::SCREEN_Z * USER_INTERFACE_POSITION::SHOW_RADIAN_ON_Y_Z 
		+ graphSizeZ * USER_INTERFACE_POSITION::SHOW_RADIAN_ON_Y_MULTI - 4)
		startZ = (int)(Screen::SCREEN_Z * USER_INTERFACE_POSITION::SHOW_RADIAN_ON_Y_Z 
		+ graphSizeZ * USER_INTERFACE_POSITION::SHOW_RADIAN_ON_Y_MULTI - 4);
	DrawBox(startX, startZ,
		(int)(startX + graphSizeX * USER_INTERFACE_POSITION::SHOW_RADIAN_ON_Y_MULTI),
		(int)(startZ + 4),
		Cr,TRUE);

	/*����CD*/
	Cr = GetColor(0, 255, 0);//�F��ݒ肷��
	int realX;
	if (ship->ReferMainWeaponCD() >= ship->ReferMainWeaponCoolDownTime())
		realX = USER_INTERFACE_POSITION::SHOOT_CD_MAIN_SIZE_X;
	else
		realX = (int)(((double)ship->ReferMainWeaponCD() /
		(double)ship->ReferMainWeaponCoolDownTime())*
			USER_INTERFACE_POSITION::SHOOT_CD_MAIN_SIZE_X);

	DrawBox(
		(int)(USER_INTERFACE_POSITION::SHOOT_CD_MAIN_X * Screen::SCREEN_X),
		(int)(USER_INTERFACE_POSITION::SHOOT_CD_MAIN_Z * Screen::SCREEN_Z),
		(int)(USER_INTERFACE_POSITION::SHOOT_CD_MAIN_X * Screen::SCREEN_X) +
		realX,
		(int)(USER_INTERFACE_POSITION::SHOOT_CD_MAIN_Z * Screen::SCREEN_Z) +
		(int)USER_INTERFACE_POSITION::SHOOT_CD_MAIN_SIZE_Z,
		Cr, TRUE
	);

	/*�Ǐ��*/
	radian = ship->ReferChangingRadian() + MathAndPhysics::PI * 1.25;
	DrawRotatePicture(UI_LIST::RUDDER,
		USER_INTERFACE_POSITION::RUDDER_X,
		USER_INTERFACE_POSITION::RUDDER_Z,
		USER_INTERFACE_POSITION::RUDDER_MULTI,
		radian);

	/*���x�w��*/
	radian = MathAndPhysics::PI * 0.65 +//�͂��߂̈ʒu
		(ship->ReferSpeedOnZ() / MathAndPhysics::MAX_SPEED_FOR_SHIP) *
		MathAndPhysics::PI * 1.35 * 1.5;
	DrawRotatePicture(UI_LIST::ARROW_WHITE,
		USER_INTERFACE_POSITION::ARROW_WHITE_SPEED_X,
		USER_INTERFACE_POSITION::ARROW_WHITE_SPEED_Z,
		USER_INTERFACE_POSITION::ARROW_WHITE_SPEED_MULTI,
		radian);

	/*�o�͎w��*/
	radian = MathAndPhysics::PI * 0.65 + //�����͐������p�x���J�n���邽�߂̊p�x
		abs(ship->ReferOutPutRate() / 1.0) *
		MathAndPhysics::PI * 1.12;//�\���͈͂��w�肵�A�p�x���v�Z����
	DrawRotatePicture(UI_LIST::ARROW_RED_2,
		USER_INTERFACE_POSITION::ARROW_WHITE_ENGINE_X,
		USER_INTERFACE_POSITION::ARROW_WHITE_ENGINE_Z,
		USER_INTERFACE_POSITION::ARROW_WHITE_ENGINE_MULTI,
		radian);

	radian = MathAndPhysics::PI * 0.65 +
		abs(ship->ReferEngineOutput() / ship->ReferEngineMaxOutput()) *
		MathAndPhysics::PI * 1.12;//��ނ��Ă��o�͂�\���ł���悤�ɂ���
	DrawRotatePicture(UI_LIST::ARROW_WHITE,
		USER_INTERFACE_POSITION::ARROW_WHITE_ENGINE_X,
		USER_INTERFACE_POSITION::ARROW_WHITE_ENGINE_Z,
		USER_INTERFACE_POSITION::ARROW_WHITE_ENGINE_MULTI,
		radian);

	/*�����߂郂�[�h��Ԃ̒�*/
	lamps[LAMP_LIST::RETURN_TO_MIDDLE].SetOnOrOff(ship->ReferReturnOn());
	//�ǂ̎����߂郂�[�h�̏�Ԃ𒲂ׂă����v�̏�Ԃ𒲐�����
	lamps[LAMP_LIST::RETURN_TO_MIDDLE].Draw();
}

void UserInterface::DrawUIUnderShip(bool lock, Coordinate2D<double> coord
	, Camera camera, double radian) {
	if (lock)
		DrawCircle(coord,camera);
	if (!lock)
		DrawMyCircle(camera, radian);
}

void UserInterface::DrawShipOnTheMap(double X, double Z,bool enemy) {
	
	unsigned int enemyCr = GetColor(255, 0, 0);
	unsigned int allyCr = GetColor(0, 0, 255);

	//�G�Ǝ����̐F���Ⴄ
	unsigned int realCr = enemy ? enemyCr : allyCr;

	//���ۂ̈ʒu���v�Z����
	double realX = abs((X + MapSize::xSize) /
		(MapSize::xSize * 2)) * mapPaperSize * 2;
	double realZ = abs((Z + MapSize::zSize) /
		(MapSize::zSize * 2)) * mapPaperSize * 1.2f;
	
	/*�~�j�}�b�v�ɓ_��`��*/
	DrawBox(
		Screen::SCREEN_X / 2 - (int)mapPaperSize + (int)realX - 3,
		Screen::SCREEN_Z - (int)(((1.2 * (int)mapPaperSize) - realZ)),
		Screen::SCREEN_X / 2 - (int)mapPaperSize + (int)realX + 3,
		Screen::SCREEN_Z - (int)(((1.2 * (int)mapPaperSize) - realZ) - 4),
		realCr,TRUE);
}

/*����̍��W���g���ĕ`��*/
void UserInterface::DrawPictureByLeftUp(int SN, double X, 
	double Z, double Multiple) {
	int startX, startZ;
	int graphSizeX, graphSizeZ;

	/*���W�A�T�C�Y���v�Z����*/
	GetGraphSize(*handleList[SN], &graphSizeX, &graphSizeZ);
	startX = (int)(Screen::SCREEN_X * X);
	startZ = (int)(Screen::SCREEN_Z * Z);
	/*�摜��`��*/
	DrawExtendGraph(
		startX, startZ,
		(int)(startX + graphSizeX * Multiple),
		(int)(startZ + graphSizeZ * Multiple),
		*handleList[SN], TRUE
	);
}

/*���S�̂w���W�Ə�̂y���W���g���ĕ`��*/
void UserInterface::DrawPictureByCenterOnX(int SN, double X,
	double Z, double Multiple) {
	int startX, startZ;
	int graphSizeX, graphSizeZ;

	/*���W�A�T�C�Y���v�Z����*/
	GetGraphSize(*handleList[SN], &graphSizeX, &graphSizeZ);
	startX = (int)((Screen::SCREEN_X * X) -
		(graphSizeX * Multiple) * 0.5);
	startZ = (int)(Screen::SCREEN_Z * Z);
	DrawExtendGraph(
		startX, startZ,
		(int)(startX + graphSizeX * Multiple),
		(int)(startZ + graphSizeZ * Multiple),
		*handleList[SN], TRUE
	);
}

/*���S���W�𗘗p���ĕ`��*/
void UserInterface::DrawPictureByCenter(int SN, double X,
	double Z, double Multiple) {
	int startX, startZ;
	int graphSizeX, graphSizeZ;

	/*���W�A�T�C�Y���v�Z����*/
	GetGraphSize(*handleList[SN], &graphSizeX, &graphSizeZ);
	startX = (int)((Screen::SCREEN_X * X) -
		(graphSizeX * Multiple) * 0.5);
	startZ = (int)((Screen::SCREEN_Z * Z) -
		(graphSizeZ * Multiple) * 0.5);
	DrawExtendGraph(
		startX, startZ,
		(int)(startX + graphSizeX * Multiple),
		(int)(startZ + graphSizeZ * Multiple),
		*handleList[SN], TRUE
	);
}

/*����O���t��`��-����_�͍���*/
void UserInterface::DrawRotatePicture(int SN, double X, double Z, double Multiple,
	double Radian) {
	int startX, startZ;
	int graphSizeX, graphSizeZ;

	startX = (int)(Screen::SCREEN_X * X);
	startZ = (int)(Screen::SCREEN_Z * Z);

	GetGraphSize(*handleList[SN], &graphSizeX, &graphSizeZ);
	DrawRotaGraph3(
		startX,startZ,
		0, 0,
		Multiple,
		Multiple,
		Radian, *handleList[SN], TRUE, FALSE
	);
}

/*����O���t��`��-����_�͒��S*/
void UserInterface::DrawRotatePicture2(int SN, double X, double Z, double Multiple,
	double Radian) {
	int graphSizeX, graphSizeZ;

	GetGraphSize(*handleList[SN], &graphSizeX, &graphSizeZ);

	DrawRotaGraph3(
		(int)X, (int)Z,
		graphSizeX / 2, graphSizeZ / 2,
		Multiple,
		Multiple,
		Radian, *handleList[SN], TRUE, FALSE
	);
}

void UserInterface::DrawBoxWithPerAndCr(Coordinate2D<double> LeftUp,
	Coordinate2D<double>Size,
	double Percentage, int Cr) {
	DrawBox((int)LeftUp.x, (int)LeftUp.z, (int)(LeftUp.x + Size.x*Percentage),
		(int)(LeftUp.z + Size.z), Cr, TRUE);
}

/*���b�N�̉~��`��*/
void UserInterface::DrawCircle(Coordinate2D<double> coord, Camera camera) {
	DrawRotatePicture2(UI_LIST::LOCK_CIRCLE, 
		(coord.x - camera.ReferRealCameraX())*camera.ReferZoomRatio(), 
		(coord.z - camera.ReferRealCameraZ())*camera.ReferZoomRatio(),
		USER_INTERFACE_POSITION::LOCK_CIRCLE * camera.ReferZoomRatio(),
		radian_returnTheLockCircle);
	radian_returnTheLockCircle += MathAndPhysics::PI / 720;
}

void UserInterface::DrawHPBar(Coordinate2D<double> coord, int HP, int maxHP) {
	Coordinate2D<double> plus = { 73,8 };//�b�t�h�̉摜�T�C�Y�ɍ��킹��
	DrawBoxWithPerAndCr(coord, plus, 1, CrBox[BLACK]);

	unsigned int tempCr;
	double per;
	per = (double)HP / (double)maxHP;

	if (per > 0.75)
		tempCr = CrBox[MEDIUM_SEA_GREEN];
	else if (per > 0.5)
		tempCr = CrBox[MY_SIN];
	else if (per > 0.25)
		tempCr = CrBox[TENNE];
	else
		tempCr = CrBox[GRENADIER];

	DrawBoxWithPerAndCr(coord, plus, per, tempCr);
}

/*�����̂����ɂ���~��`��*/
void UserInterface::DrawMyCircle(Camera camera, double radian) {
	int xOnScreen;
	int zOnScreen;

	xOnScreen = Screen::SCREEN_X / 2;
	zOnScreen = Screen::SCREEN_Z / 2;

	SetTrans(120);
	DrawRotatePicture2(UI_LIST::MY_DIRECT,
		xOnScreen, zOnScreen, 
		USER_INTERFACE_POSITION::MY_DIRECT_MULTI * camera.ReferZoomRatio(),
		radian);
	ResetTrans();
}

void UserInterface::SetColorBox() {
	CrBox[GREEN] = GetColor(0, 255, 0);
	CrBox[RED] = GetColor(255, 0, 0);
	CrBox[BLUE] = GetColor(0, 0, 255);
	CrBox[BLACK] = GetColor(0, 0, 0);
	CrBox[WHITE] = GetColor(255, 255, 255);
	CrBox[MIDNIGHT] = GetColor(33, 47, 61);
	CrBox[GRENADIER] = GetColor(203, 67, 53);
	CrBox[LOCHMARA] = GetColor(36, 113, 163);
	CrBox[MY_SIN] = GetColor(245, 176, 65);
	CrBox[TENNE] = GetColor(211, 84, 0);
	CrBox[MEDIUM_SEA_GREEN] = GetColor(46, 204, 113);
}