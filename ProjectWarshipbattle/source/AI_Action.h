#pragma once
/*���̃w�b�_�[�t�@�C���ɕۑ����Ă���̂�AI�̍s���w�ߕ\*/

enum AI_MOVE_FLAGSHIP{
	PATROL = 0,//�p�g���[��//xunluo
	/*CYCLE�̕����͔��q��̎��g��*/
	APPROACH_OUT_OF_BATTLE_CYCLE,
	APPROACH_OUT_OF_BATTLE,
	APPROACH_IN_BATTLE_CYCLE,
	APPROACH_IN_BATTLE,
	IN_BATTLE_CYCLE,
	IN_BATTLE
};

enum AI_FLAGSHIP_SPEED {
	BATTLE_SPEED_NORMAL,//���ʂɐ퓬���s�����̑��x
	BATTLE_SPEED_ESCAPE_PURSUIT,//�����鎞�̑��x
	PARTORL //�p�g���[�����̑��x
};

enum BATTLE_STATUS {
	APPROACHING,//���q��
	GOING_FAR_AWAY//���q��
};

enum AI_FIGHT_FLAGSHIP {
	CONCENTRATE_ON_FLAGSHIP,//�G�̃t���O�V�[�v�ɏW������
	CONCENTRATE_ON_MY_MARK,//�����̃^�[�Q�b�g�Ɠ����ɂ���
	FREE_FIRE_KILL_DAMAGED_FIRST,//���R�ˌ�-�_���[�W���󂯂��G��D��ڕW�ɂ���
	FREE_FIRE_SHOOT_INTACT_FIRST,//���R�ˌ�-�������󂯂Ȃ������G��D��ڕW�ɂ���
	FREE_FIRE_SHOOT_CLOSE,//���R�ˌ�-��ԋ߂��ڕW���^�[�Q�b�g�ɂ���
	FREE_FIRE_SHOOT_FAR//���R�ˌ�-��ԉ����ڕW���^�[�Q�b�g�ɂ���
};

enum LINE_UP {
	VERTICAL_LAYOUT,//�c�^�w
	//���w
	//�΂ߐw
};