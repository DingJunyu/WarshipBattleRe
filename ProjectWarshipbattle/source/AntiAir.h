#pragma once
class AntiAir
{
public:
	AntiAir();
	~AntiAir();
	
private:
	//�p�[�c��HP
	int heartPoint;
	int MaxHeartPoint;
	//�p�[�c�̑��Έʒu�ƃT�C�Y
	double relativeCoordX;
	double relativeCoordZ;
	//�L���͈�
	double horizontalRange;
	double verticalRange;

	int ammoRemain;
	double hitRate;
};

