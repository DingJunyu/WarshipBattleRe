#pragma once
#define MORALE_DOWN_WITH_MAINPARTDAMAGE 10
#define MORALE_DOWN_AFTER_MAINPARTDAMAGECOUNT 5
#define MORALE_DOWN_WITH_SUBPARTDAMAGE 5
#define MORALE_DOWN_AFTER_SUBPARTDAMAGECOUNT 10
/*�m�C���������邽�߂̃N���X*/
class Morale
{
public:
	Morale();
	~Morale();

	void CountMainPartDamage() { mainPartDamageCount++; }
	void CountSubPartDamage() { subPartDamageCount++; }
	void CheckMorale();	

private:
	int morale;
	bool movable;//�m�C������Ȃ��ƑD�͓����Ȃ�
	int mainPartDamageCount;
	int subPartDamageCount;
};