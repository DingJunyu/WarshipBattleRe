#pragma once
#include"DxLib.h"
#include"DefinedData.h"
#include<time.h>
#include<cmath>
class Effect
{
public:
	Effect();
	Effect(bool Movable, int continueTime, double Radian,
		double TargetRadian, double RadianChangePerFrame, double Speed, double CoordX,
		double CoordZ, int *GraphicHandle, bool Spread, 
		double ZoomMutliple, double ZoomRate) : movable(Movable),
		timeUp(false), endTime(GetNowCount() + continueTime),
		radian(Radian), targetRadian(TargetRadian),
		radianChangePerFrame(RadianChangePerFrame),
		speed(Speed), coordX(CoordX), coordZ(CoordZ), graphicHandle(GraphicHandle),
		spread(Spread), zoomMutliple(ZoomMutliple), zoomRate(ZoomRate)
	{
		right = rand() % 2;
		GetGraphSize(*graphicHandle, &graphX, &graphZ);
	}
	~Effect();

	void Draw(int x, int z);
	void Move();

	//���Ԃɉ߂��������
	bool ReferTimeUp() { return timeUp; }
	void Update() { Check(); }

	void SetGraphHandle(int *GH) { graphicHandle = GH; }

private:
	void Check();//���Ԃ��؂ꂽ�����m�F
	//�G�t�F�N�g�̃^�C�v�ƈړ��ł��邩
	bool movable;
	int type;
	//���W�Ȃ�
	double radian;//���W�A��
	double targetRadian;//�ڕW���W�A��
	double radianChangePerFrame;//���t���[���̕ύX�p�x
	double coordX;
	double coordZ;
	double speed;//�ړ����x
	bool right;//�E�Ɉړ����邩
	//�摜�f�[�^
	int *graphicHandle;
	int graphX;
	int graphZ;
	int *animationHandle;
	//��������
	int endTime;
	bool timeUp;

	bool spread;//�������g��ł��邩
	double zoomMutliple;//�{��
	double zoomRate;//�t���[�����Ƃ̊g�嗦
};