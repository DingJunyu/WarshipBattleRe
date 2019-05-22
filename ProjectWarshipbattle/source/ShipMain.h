#pragma once
#include"AllMovableObjects.h"//���N���X
#include"ShipCrashParts.h"//�g�o�v�Z�p�p�[�c
#include"DefinedData.h"//�萔
#include"IncludeForAll.h"
#include"Weapon.h"//����
#include"Camera.h"//�J����
#include"EffectPoint.h"//�G�t�F�N�g������
#include"ShipData.h"
#include"Engine.h"//�G���W��
#include"EffectTemplate.h"//�G�t�F�N�g�����평�����e��
#include"SoundLoader.h"//�������[�_�[
#include"FireController.h"//�ˌ��Ǘ�
#include"Lock.h"//���b�N���鎞�Ɋւ�鑀���f�[�^
#include<stdio.h>
#include<stdlib.h>
class ShipMain :
	public AllMovableObjects
{
public:
	ShipMain() : AllMovableObjects(false, false, false, true) {
		currentEngineOutPutRate = 0;
		returnToCenter = false;

		forecastSeconds = 0;
		reviseRadianOnY = 0;
		reviseRadianOnZ = 0;

		/*�e�X�g����*/
		hitPoint = 240;
		shipMainCrash = { 180, 40, 18 };
		controled = false;
		canIShoot = false;

		flameAfterSink = 0;
	}
	~ShipMain();

	//������
	void RegistrateShipCrashParts();
	void InifThisShip(int *ShipHandle, int *SShadowH, int ShipNum,
		EffectTemplate ET, SoundLoader *SL);

	bool InifThisShip(PictureLoader *PL, EffectTemplate ET,
		SoundLoader *SL, int num, int SN);
	void SetThisShipPos(Coordinate<double> Coord, double radian);

	void SetEffectPoint(ShipData SD);
	void DestroyMemory();
	void SetWeaponTest(PictureLoader *PL);
	void SetWeapon();

	//���މ��o�֘A
	bool ReferSinkingEnding() { return flameAfterSink == sinkingFlame; }
	void countSinkingFlame() { flameAfterSink++; }

	//�f�[�^����֐�
	void SetSerialNumber(int x) { serialNumber = x; }

	//�R���g���[��
	void ControlThisShip(int Command);

	//�X�V
	void Update();

	void SetControled() { controled = !controled; }

	//�ړ��֘A
	void SetEngineOutPutRate(bool up);
	void SetEngineOutPutRate(double oP) {
		currentEngineOutPutRate = oP;
		mainEngine.SetOutPutPercentage(currentEngineOutPutRate);//���̏o�͂�^����
	}
	void CalSpeed();
	void ChangeDirect(bool right);
	void ReturnDirectChange() { returnToCenter = !returnToCenter; }
	void Alignment();

	void SetChangingDirect(double rad) {
		currentRadian = rad;
		if (rad > maxRadian)
			currentRadian = maxRadian;
		if (rad < -maxRadian)
			currentRadian = -maxRadian;
		SetRadianChangePerFrame(currentRadian);
	}

	void TEST();
	void TestDraw(double x, double z);

	/*�ȒP�������퓬*/
	void SufferDamage(int damage);
	/*���b�N�I���̏�Ԃ̑���*/
	void ChangeForecastSecond(bool up);
	void ChangeReviseRadianOnY(bool up);
	void ChangeReviseRadianOnZ(bool right);
	void ResetReviseData() {
		forecastSeconds = 0;
		reviseRadianOnY = 0;
	}

	//�₢���킹
	int ReferShipType() { return shipType; }
	const char * ReferName() { return name; }
	int ReferMaxHP() { return maxHitPoint; }
	int ReferHP() { return hitPoint; }
	/*�ړ��֘A*/
	bool ReferReturnOn() { return returnToCenter; }
	double ReferChangingRadian() { return currentRadian; }
	double ReferOutPutRate() { return currentEngineOutPutRate; }
	/*����֘A*/
	int ReferWeaponCount(bool Main) {
		if (Main)
			return MainWeaponCount;
		else
			return SubWeaponCount;
	}
	double ReferMainWeaponRadianOnZ() {
		return MainWeapon[0].ReferRadianOnZ();
	}
	double ReferMainWeaponCD() {
		return
			GetTickCount() - MainWeapon[0].ReferLastShootedTime();
	}
	double ReferMainWeaponCoolDownTime() {
		return MainWeapon[0].ReferReloadTime();
	}
	double ReferMainWeaponRadianOnY() {
		return MainWeapon[0].ReferRadianOnY();
	}
	double ReferMainMaxWeaponRadianOnY() {
		return MainWeapon[0].ReferMaxRadianOnY();
	}
	double ReferMainMaxRange() { return fireControllerMain.ReferMaxRange(); }
	int ReferDamage() { return MainWeapon[0].ReferDamage(); }

	/*collision�֘A*/
	double ReferShipCrashR() { return shipCrashR; }
	CrashSize3d<double> ReferShipCrashSize() { return shipMainCrash; }
	int ReferCollisionPointAmount() { return pointCount; }
	Coordinate2D<double> ReferCollisionPoint(int num) {
		return aroundPointPos[num];
	}
	void ResetStatus() {
		currentRadian = 0;
		currentEngineOutPutRate = 0;
		mainEngine.SetOutPutPercentage(currentEngineOutPutRate);
	}

	bool ReferControled() { return controled; }
	/*�ԍ�*/
	int ReferSerialNumber() { return serialNumber; }
	//�G���W���֘A
	double ReferEngineOutput() { return mainEngine.ReferOutput(); }
	double ReferEngineMaxOutput() { return mainEngine.ReferMaxOutput(); }
	/*���b�N�֌W*/
	int ReferForecastSecond() { return forecastSeconds; }
	bool ReferCanIShoot() { return canIShoot; }

	//�G�t�F�N�g����
	Effect NewBubble(int num);
	Effect NewSmoke(int num);
	int ReferBubblePointCount() { return bubblePointCount; }
	int ReferSmokePointCount() { return smokePointCount; }

	void CheckAndPlaySound();

	/*�ˌ�*/
	int HowManyWeaponYouHave() { return MainWeaponCount; }
	bool IsThisOneUsable(int Num, bool Main);
	Ammo Shoot(int Num, bool Main);
	void ShowMePointOfImpact(Camera camera);


	void LockAndAim(ShipMain *ship,bool render);

	/*���b�N*/
	Lock fireDataFigureUp;

private:
	ShipCrashParts * MainParts;
	ShipCrashParts * SubParts;
	double shipCrashR;
	CrashSize3d<double> shipMainCrash;
	/*�����̒e�������ɓ�����Ȃ��悤�Ɏg�p����ԍ��ł�*/
	int serialNumber;
	int shipType;
	bool controled;

	Weapon * MainWeapon;//���C������
	int MainWeaponCount;//���C������̐�
	Weapon * SubWeapon;//�T�u����
	int SubWeaponCount;//�T�u����̐�

	EffectPoint * bubbleStartPoint;//���A�����|�C���g
	int bubblePointCount;//���A�����|�C���g�̐�

	/*�����蔻��E�A�����p�_*/
	Coordinate2D<double> aroundPointPos[AUTO_SHIP_DATA::AROUND_POINT];
	int pointCount;//�G�t�F�N�g�����|�C���g�̐�
	void FindThosePoint();

	EffectPoint * smokeStartPoint;
	int smokePointCount;//�_�����|�C���g�̐�

	void MemorySecure();

	/*�G���W��*/
	Engine mainEngine;
	double currentEngineOutPutRate;//�o�͂̃p�[�Z���e�[�W

	/*�C�̃X�e�[�^�X�ύX*/
	bool TurnMainWeapon(bool right);
	bool PullMainWeapon(bool up);

	/*�ˌ��Ǘ�*/
	void CalThePoint();
	void CalMainPoint();

	void DrawMainPoint(Camera camera);
	void DrawSubPoint(Camera camera);
	
	bool canIShoot;

	double draft;//�i��:�������g�����Ɏg���f�[�^�ł��B
	int thisShipType;

	int *shipHandle[SerialNumberOfShipHandle::ARRAY_SIZE];

	//������
	double maxRadian;
	double currentRadian;
	double radianChangePerFrame;
	bool returnToCenter;
	
	char name[100];

	/*�����֘A*/
	/*�z�[�h�̎�ނ͂܂����܂��ĂȂ��̂ō��͎g���Ă��Ȃ�*/
	/*soundloader�̍\�����X�V������܂��X�V����*/
	void LoadSound(SoundLoader *SL);
	int *soundEngine;
	int *soundMoving;
	int *soundSlow;
	int *soundTurning;
	int HordType;
	int *soundHordHigh;
	int *soundHordLow;

	/*�ȒP�������퓬����*/
	int hitPoint;
	int maxHitPoint;

	/*�ˌ�*/
	FireController fireControllerMain;
	FireController fireControllerSub;
	double targetRadianForMain;
	double targetRadianForSub;
	const int framesCountInaSecond = 60;//���b�t���[����
	const int maxForecastSecond = 15;
	int forecastSeconds;//���b��̈ʒu�𐄑�������
	Coordinate2D<double> nextPos;
	double reviseRadianOnY;
	double reviseRadianOnZ;
	const double maxreviseRadianOnY = MathAndPhysics::PI * (15.0 / 180.0);
	const double maxReviseRadianOnZ = MathAndPhysics::OneDegreeRadian * 30;

	void CalNextPos(ShipMain *ship);
	void CalDistance(ShipMain *ship);

	double distance;
	double targetRadianOnZforMain;

	const int sinkingFlame = 360;
	int flameAfterSink;

	const float lockCircleR = 70;
	const int lockCirclePosNum = 70;
	const float lockCircleThick = 2.0;

};