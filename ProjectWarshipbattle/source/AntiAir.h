#pragma once
/*�D�ɏ悹���΋󕐊���������邽�߂̃N���X*/
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

	int ammoRemain;//�c�e��
	double hitRate;//�ˌ����x
};

