#pragma once
#include "AllMovableObjects.h"
class Ammo :
	public AllMovableObjects
{
public:
	Ammo() : AllMovableObjects(true, //��ׂ����
		false, //��s�@�ł͂Ȃ�
		true, //�_�Ƃ��Čv�Z����
		true) {//�����蔻�肪�L��
		isUsable = true;
		damage = 5;
	}
	/*���������ɕK�v�Ȃ���:
	���x�A�p�x�A�w�y�x���W�A�x���̊p�x�A�e�̃n���h���A�D�̔ԍ�*/
	Ammo(double Speed, double Radian, double CoordX, double CoordZ, double CoordY,
		double RadianOnY, int *AmmoHandle,int SerialNumber) :
		AllMovableObjects(true, false, true, true) {
		SetRadianOnZ(Radian);
		NewCoordX(CoordX);
		NewCoordZ(CoordZ);
		NewCoordY(CoordY);
		SetRadianOnY(RadianOnY);
		SetSpeed(Speed);
		ammoHandle = AmmoHandle;
		serialNumber = SerialNumber;
		SetData();//�摜�T�C�Y�����

		isUsable = true;
		damage = 10;
	}
	~Ammo();

	double ReferDamage() { return damage; }
	void CheckAlive();//�C�ɗ������̂�
	bool ReferUsable() { return isUsable; }
	void SetData();//�摜�̃T�C�Y��ǂݍ���
	void Unusable() { isUsable = false; }
	int ReferSerialNumber() { return serialNumber; }
	
	//�ʏ�`��
	void Draw(int x,int z);
	//�����O�̓��扉�o
	void DrawDelete(int x, int z);
	bool FallIntoWater() { 
		if (ReferCoordY() <= 0)return true; 
		else return false;
	}
	
private:
	double penetrateDepth;//�ђʂł���A�[�}�[�̍ő����
	double damage;//�_���[�W

	bool isUsable;//�܂������邩

	/*�摜�T�C�Y*/
	int graphX;
	int graphZ;

	/*�摜�n���h��*/
	int *ammoHandle;

	/*���ʔԍ�*/
	int serialNumber;
};

