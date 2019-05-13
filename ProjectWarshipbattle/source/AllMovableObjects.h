#pragma once
#include<cmath>
#include"DefinedData.h"
#include"Camera.h"
#include"DxLib.h"
/*�C���^�[�t�F�C�X�ɂ���:
�ꕔ���̃C���^�[�t�F�C�X��private�Ɉڂ�����
*/

class AllMovableObjects
{
public:
	//��ׂ邩�A��s�@���A�_�Ɍ����邩�A�����蔻��g����
	AllMovableObjects(bool Flyable, bool Plane, bool Point,bool Crashable) :
		flyable(Flyable),
		plane(Plane), alive(true), point(Point), crashable(Crashable) {
		alive = true;
		inList = false;
		radianChangePerFrame = 0;
		airResistanceActive = false;
		airResistance = 0.00001;
		speedOnZ = 0;
		input = 0;
		distanceMoved = 0;
	}
	virtual ~AllMovableObjects();

	//�f�[�^�₢���킹�֐�
	double ReferCoordX() { return coord.x; }/*x���W*/
	double ReferCoordY() { return coord.y; }//y���W
	double ReferCoordZ() { return coord.z; }//z���W
	Coordinate<double> ReferCoord() { return coord; }//3D���W
	Coordinate2D<double> ReferCoord2D_d() { return Coordinate2D<double> 
	{(double)coord.x, (double)coord.z}; }//���ʍ��W(double)
	Coordinate2D<int> ReferCoord2D() { return Coordinate2D<int>
	{(int)coord.x,(int)coord.z}; }/*���ʍ��W(int)*/

	double ReferSpeedOnZ() { return speedOnZ; }//���ʑ��x
	double ReferSpeedOnY() { return speedOnY; }//�������x
	double ReferAirResistance() { return airResistance; }//��C��R
	double ReferRadianOnZ() { return radianOnZ; }//���ʊp�x
	double ReferRadianOnY() { return radianOnY; }//�����p�x
	double ReferLength() { return length; }//����(�摜)
	double ReferWidth() { return width; }//��(�摜)
	double ReferMultiple() { return multiple; }//�g�嗦

	double ReferDistanceMoved() { return distanceMoved; }

	//��Ԗ₢���킹�֐�
	bool ReferAlive() { return alive; }//������󋵊m�F
	bool ReferInList() { return inList; }
	int *ReferGraphHandle() { return pictureHandle; }//�摜�n���h��

	//�ړ��֐�
	void Move();//�ړ�
	void Unmove();//�O�̏�Ԃɖ߂�
	void FallingDown();//����
	void SpeedDownbyAirResistance();//��C��R�̉e���ő��x�����Ƃ�

	//�`��֐�
	void SetPictureHandle(int *p) { pictureHandle = p; }
	void SetShadowHandle(int *p) { shadowHandle = p; }
	void Draw(Camera CM);//�J�����̒��S�ɂ��鎞�g���`��֐�
	void DrawShadow(Camera CM);//�e��`�悷��
	void DrawSub(Camera CM);//���̂Ƃ���ɂ��鎞�Ɏg���`��֐�
	void DrawSubShadow(Camera CM);//�e��`�悷��
	void DrawCard(Coordinate2D<double> coord);

	//��Ԑݒu�֐�
	//�X�e�[�^�X�ݒ�֐�
	//�g���P��
	void SetMass(double Mass) { mass = Mass; }//�d�ʂ�ݒu����
	//�j���[�g���P�ʂł����A�����Ƃ�����ƈႤ
	void SetOutput(double Output) { input = Output; }//�o�͂�ݒu����
	void SetMaxOutput(double MaxOutput) { maxInput = MaxOutput; }//�ő�o�͒l��ݒu����
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

	void Killed() { alive = false; SetSpeed(0); }//��Ԃ����S�ɐݒu����
	void PutInList() { inList = true; }
	void SetSpeed(double Spe);//���x��ݒu
	void SetMaxSpeedOnZ(double Speed) { maxSpeedOnZ = Speed; }//�ő呬�x��ݒu
	void UnsetPlane() { plane = false; }//��s�@�����񂾂畁�ʂ̔�Ԃ��̂ɕς���
	

private:
	/*XZ�͐������W�AY�͍���*/
	Coordinate<double> coord;
	Coordinate<double> oldCoord;
	double distanceMoved;
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
	bool inList;

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
	/*�T�C�Y*/
	double length;
	double width;

	//�����蔻�肪�K�v���ǂ���
	bool crashable;
	
	//�摜�̃n���h��
	int *pictureHandle;
	int *shadowHandle;
	double multiple;
};

