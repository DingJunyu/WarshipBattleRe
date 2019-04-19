#pragma once
#define MORALE_DOWN_WITH_MAINPARTDAMAGE 10
#define MORALE_DOWN_AFTER_MAINPARTDAMAGECOUNT 5
#define MORALE_DOWN_WITH_SUBPARTDAMAGE 5
#define MORALE_DOWN_AFTER_SUBPARTDAMAGECOUNT 10
/*士気を実現するためのクラス*/
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
	bool movable;//士気が足らないと船は動かない
	int mainPartDamageCount;
	int subPartDamageCount;
};