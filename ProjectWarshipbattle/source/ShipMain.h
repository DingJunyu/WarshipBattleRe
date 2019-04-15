#pragma once
#include"AllMovableObjects.h"
#include"ShipCrashParts.h"
#include"DefinedData.h"
#include"IncludeForAll.h"
#include"Weapon.h"
#include"Camera.h"
#include"EffectPoint.h"
#include"ShipData.h"
#include"Engine.h"
#include"EffectTemplate.h"
#include"SoundLoader.h"
class ShipMain :
	public AllMovableObjects
{
public:
	ShipMain() : AllMovableObjects(false, false, false, true) {
		currentAccPercentage = 0;
		returnToCenter = false;

		/*�e�X�g����*/
		hitPoint = 100000;
		shipMainCrash = { 245, 30, 24 };
	}
	~ShipMain();

	//������
	void RegistrateShipCrashParts();
	void InifThisShip(int *ShipHandle,int *SShadowH,int ShipNum, 
		EffectTemplate ET,SoundLoader *SL);
	void SetEffectPoint(ShipData SD);
	void DestroyMemory();
	void SetWeaponTest(PictureLoader *PL);
	void SetWeapon();

	//�f�[�^����֐�
	void SetSerialNumber(int x) { serialNumber = x; }

	//�R���g���[��
	void ControlThisShip(int Command);

	//�ړ��֘A
	void ChangeAccPercentage(bool up);
	void CalSpeed();
	void ChangeDirect(bool right);
	void ReturnDirectChange() { returnToCenter = ! returnToCenter; }
	void Alignment();

	void TEST();
	void TestDraw(double x,double z);

	/*�ȒP�������퓬*/
	void SufferDamage(int damage);

	//�₢���킹
	/*�ړ��֘A*/
	bool ReferReturnOn() { return returnToCenter; }
	double ReferChangingRadian() { return currentRadian; }
	double ReferOutPutRate() { return currentAccPercentage; }
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
	/*collision�֘A*/
	double ReferShipCrashR() { return shipCrashR; }
	CrashSize3d<double> ReferShipCrashSize() { return shipMainCrash; }
	int ReferCollisionPointAmount() { return pointCount; }
	Coordinate2D<double> ReferCollisionPoint(int num) {
		return aroundPointPos[num];
	}
	/*�ԍ�*/
	int ReferSerialNumber() { return serialNumber; }
	//�G���W���֘A
	double ReferEngineOutput() { return mainEngine.ReferOutput(); }
	double ReferEngineMaxOutput() { return mainEngine.ReferMaxOutput(); }

	//�G�t�F�N�g����
	Effect NewBubble(int num);
	Effect NewSmoke(int num);
	int ReferBubblePointCount() { return bubblePointCount; }

	void CheckAndPlaySound();

	/*�ˌ�*/
	int HowManyWeaponYouHave() { return MainWeaponCount; }
	bool IsThisOneUsable(int Num, bool Main);
	Ammo Shoot(int Num, bool Main);



private:
	ShipCrashParts * MainParts;
	ShipCrashParts * SubParts;
	double shipCrashR;
	CrashSize3d<double> shipMainCrash;
	/*�����̒e�������ɓ�����Ȃ��悤�Ɏg�p����ԍ��ł�*/
	int serialNumber;

	Weapon * MainWeapon;//���C������
	int MainWeaponCount;//���C������̐�
	Weapon * SubWeapon;//�T�u����
	int SubWeaponCount;//�T�u����̐�

	EffectPoint * bubbleStartPoint;//���A�����|�C���g
	int bubblePointCount;//���A�����|�C���g�̐�

	/*�����蔻��E�A�����p�_*/
	Coordinate2D<double> aroundPointPos[AUTO_SHIP_DATA::AROUND_POINT];
	int pointCount;
	void FindThosePoint();

	EffectPoint * smokeStartPoint;
	int smokePointCount;

	void MemorySecure();

	/*�G���W��*/
	Engine mainEngine;
	double currentAccPercentage;//�o�͂̃p�[�Z���e�[�W

	/*�C�̃X�e�[�^�X�ύX*/
	bool TurnMainWeapon(bool right);
	bool PullMainWeapon(bool up);

	double draft;//�i��:�������g�����Ɏg���f�[�^�ł��B
	int shipType;

	int *shipHandle[SerialNumberOfShipHandle::ARRAY_SIZE];

	//������
	double maxRadian;
	double currentRadian;
	double radianChangePerFrame;
	bool returnToCenter;
	
	std::string name;

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
};