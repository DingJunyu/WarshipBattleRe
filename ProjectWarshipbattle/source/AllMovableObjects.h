#pragma once
#include<cmath>
#include"DefinedData.h"
#include"Camera.h"
#include"DxLib.h"

class AllMovableObjects
{
public:
	//��ׂ邩�A��s�@���A�_�Ɍ����邩�A�����蔻��g����
	AllMovableObjects(bool Flyable, bool Plane, bool Point,bool Crashable) :
		flyable(Flyable),
		plane(Plane), alive(true), point(Point), crashable(Crashable) {
		alive = true;
		radianChangePerFrame = 0;
		airResistanceActive = false;
		airResistance = 0.001;
		speedOnZ = 0;
	}
	virtual ~AllMovableObjects();

	//�f�[�^�₢���킹�֐�
	double ReferCoordX() { return coord.x; }
	double ReferCoordY() { return coord.y; }
	double ReferCoordZ() { return coord.z; }
	Coordinate<double> ReferCoord() { return coord; }
	double ReferSpeedOnZ() { return speedOnZ; }
	double ReferSpeedOnY() { return speedOnY; }
	double ReferAirResistance() { return airResistance; }
	double ReferRadianOnZ() { return radianOnZ; }
	double ReferRadianOnY() { return radianOnY; }
	double ReferLength() { return length; }
	double ReferWidth() { return width; }
	double ReferMultiple() { return multiple; }

	//�����蔻��p�₢���킹
	double ReferLeft() { return coord.x - width; }
	double ReferRight() { return coord.x; }
	double ReferUpper() { return coord.z - length; }
	double ReferLower() { return coord.z; }

	//��Ԗ₢���킹�֐�
	bool ReferAlive() { return alive; }
	int *ReferGraphHandle() { return pictureHandle; }

	//�ړ��֐�
	void Move();
	void Unmove();
	void FallingDown();
	void SpeedDownbyAirResistance();

	//�`��֐�
	void SetPictureHandle(int *p) { pictureHandle = p; }
	void SetShadowHandle(int *p) { shadowHandle = p; }
	void Draw(Camera CM);
	void DrawShadow(Camera CM);
	void DrawSub(Camera CM);
	void DrawSubShadow(Camera CM);

	//��Ԑݒu�֐�
	//�X�e�[�^�X�ݒ�֐�
	//�g���P��
	void SetMass(double Mass) { mass = Mass; }
	//�j���[�g���P�ʂł����A�����Ƃ�����ƈႤ
	void SetOutput(double Output) { input = Output; }
	void SetMaxOutput(double MaxOutput) { maxInput = MaxOutput; }
	//�V�������W��ݒ肷��
	void NewCoordX(double X) { coord.x = X; }
	void NewCoordZ(double Z) { coord.z = Z; }
	void NewCoordY(double Y) { coord.y = Y; }
	void SetCoord(Coordinate<double> NC) { coord = NC; }
	//�V�����p�x��ݒ肷��
	void SetRadianOnZ(double ROZ) { radianOnZ = ROZ; }
	void SetRadianChangePerFrame(double RCPF) { radianChangePerFrame = RCPF; }
	void SetRadianOnY(double ROY) { radianOnY = ROY; }
	//�T�C�Y��ݒ肷��
	void SetLength(double L) { length = L; }
	void SetWidth(double W) { width = W; }
	void SetMultiple(double M) { multiple = M; }
	void Killed() { alive = false; }
	void SetSpeed(double Spe);
	void SetMaxSpeedOnZ(double Speed) { maxSpeedOnZ = Speed; }
	void SetAirResistance(double AR) { airResistance = AR; }
	void UnsetPlane() { plane = false; }
	

private:
	/*XZ�͐������W�AY�͍���*/
	Coordinate<double> coord;
	Coordinate<double> oldCoord;
	/*�X�s�[�h�͐����Ɛ����ɕ�����*/
	double speedOnZ;
	double speedOnY;
	double maxSpeedOnZ;//�D�Ɣ�s�@�̍ő呬�x
	bool speedOnZLargerThan0;//�����ƕ����̔���
	bool airResistanceActive;//��C��R���g����
	double airResistance;//��C��R
	double radianOnZ;//�s������(����)
	double radianChangePerFrame;
	double radianOnY;//�s������(����)
	bool alive;//��������

	/*�o�͒l�Əd��*/
	double input;
	double maxInput;
	double mass;

	/*�p����Œl��^����*/
	bool flyable;//����͔�ׂ���̂��i�d�͉e���̗L���j
	bool plane;//����͔�ׂ���̂����A�d�͂��������Ĕ�Ԃ��Ƃ��ł���

	/*�����`�𗘗p���ĊȒP�ȓ����蔻����s��*/
	/*�p����Œl��^����*/
	bool point;//�_�Ɣ��f���鎞�͒����Ȃǂ��g��Ȃ�
	double length;
	double width;

	//�����蔻�肪�K�v���ǂ���
	bool crashable;
	
	//�摜�̃n���h��
	int *pictureHandle;
	int *shadowHandle;
	double multiple;
};

