#pragma once
#include"DefinedData.h"
class FlagShipAI
{
public:
	FlagShipAI();
	~FlagShipAI();

	void LetUsGo();//�ړ��֐�-�X�e�[�^�X�ɂ���Č��߂��s�����s��
	void LetUsFight();//�퓬�֐�

private:
	/*�ړ�����*/
	void SetWayPoint();//�E�F�C�|�C���g�ݒ�֐�

	/*�v�Z����*/
	void CalRadianNeeded();//�E�F�C�|�C���g�֍s�����ߕK�v�Ȋp�x�̍�
	void CalDistanceToOpponent();//����Ƃ̋������v�Z����

	/*�퓬����*/
	void ChooseTarget();//�G��I��

};

